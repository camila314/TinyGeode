#include <Geode/Geode.hpp>
#include <concepts>

extern "C" {
	#include <tiny.h>
}

using namespace geode::prelude;

namespace tiny_geode {
	template <typename T>
	struct tiny_converter;


	template <>
	struct tiny_converter<bool> {
		static Tiny_Value to(Tiny_StateThread*, bool value) {
			return Tiny_NewBool(value);
		}
		static bool from(const Tiny_Value& value) {
			return Tiny_ToBool(value);
		}
	};

	template <std::integral I>
	struct tiny_converter<I> {
		static Tiny_Value to(Tiny_StateThread*, I value) {
			return Tiny_NewInt(value);
		}
		static I from(const Tiny_Value& value) {
			return Tiny_ToInt(value);
		}
	};

	template <typename F> requires (std::same_as<float, F> || std::same_as<double, F>)
	struct tiny_converter<F> {
		static Tiny_Value to(Tiny_StateThread*, F value) {
			return Tiny_NewFloat(value);
		}
		static F from(const Tiny_Value& value) {
			return Tiny_ToFloat(value);
		}
	};

	template <std::convertible_to<std::string> S>
	struct tiny_converter<S> {
		static Tiny_Value to(Tiny_StateThread* th, S value) {
			auto str = std::string(value);
			char* dat = new char[str.size()];
			std::copy(str.begin(), str.end(), dat);

			return Tiny_NewString(th, dat, str.size());
		}
		static S from(const Tiny_Value& value) {
			return Tiny_ToString(value);
		}
	};

	template <typename P>
	struct tiny_converter<P*> {
		static Tiny_Value to(Tiny_StateThread* th, P* value) {
			static Tiny_NativeProp prop = {
				typeid(P).name(),
				nullptr,
				nullptr
			};

			return Tiny_NewNative(th, value, &prop);
		}
		static P* from(const Tiny_Value& value) {
			return reinterpret_cast<P*>(Tiny_ToAddr(value));
		}
	};

	template <>
	struct tiny_converter<void> {
		static Tiny_Value to(Tiny_StateThread*) {
			return Tiny_Null;
		}
		static void from(const Tiny_Value& value) {
			return;
		}
	};

	template <typename T>
	T parseArg(const Tiny_Value** args) {
		auto ret = tiny_converter<T>::from(**args);
		(*args)++;
		return ret;
	}

	// https://gist.github.com/khvorov/cd626ea3685fd5e8bf14#file-lambda_to_function-cpp-L15
	template <typename F>
	struct extract_fn : public extract_fn<decltype(&F::operator())> {};

	template <typename R, typename C, typename... Args>
	struct extract_fn<R (C::*)(Args...) const> {
	    using fn_type = std::function<R (Args...)>;
	};

	// https://stackoverflow.com/q/62266052
	template<unsigned N>
	struct TemplateStr {
	    char buf[N + 1]{};
	    constexpr TemplateStr(char const* s) 
	    {
	        for (unsigned i = 0; i != N; ++i) buf[i] = s[i];
	    }
	    constexpr operator char const*() const { return buf; }

	    // not mandatory anymore
	    auto operator<=>(const TemplateStr&) const = default;
	};
	template<unsigned N> TemplateStr(char const (&)[N]) -> TemplateStr<N - 1>;


	class TinyBaseNode : public CCNode {
	 	inline static std::set<const Tiny_State*> states;

		template <TemplateStr name, typename R, typename ...Args>
		void _bindFunction(std::function<R(Args...)> f) {
			static std::map<std::pair<const Tiny_State*, std::string>, std::function<R(Args...)>> mp;

			std::set<std::pair<const Tiny_State*, std::string>> badKeys;
			for (auto& [k, v] : mp) {
				if (states.find(k.first) == states.end()) {
					badKeys.insert(k);
				}
			}

			for (auto& k : badKeys)
				mp.erase(k);

			mp[std::pair {const_cast<const Tiny_State*>(m_state), (char const*)name}] = f;

			Tiny_ForeignFunction fn = +[](Tiny_StateThread* th, const Tiny_Value *args, int count) -> Tiny_Value {
				std::pair tmp{th->state, (char const*)name};
				if constexpr (std::is_same_v<R, void>) {
					mp[tmp](parseArg<Args>(&args)...);
					return Tiny_Null;
				} else {
					return tiny_converter<R>::to(th, mp[tmp](parseArg<Args>(&args)...));
				}
			};

			Tiny_BindFunction(m_state, name, fn);
		}

	 protected:
	 	Tiny_State* m_state;
	 	Tiny_StateThread m_thread;
	 public:
		inline static TinyBaseNode* create() {
			auto pRet = new TinyBaseNode();
			
			if (pRet && pRet->CCNode::init()) {
				if (pRet->init()) {
					pRet->autorelease();
					return pRet;
				}
			}
			
			CC_SAFE_DELETE(pRet);
			return NULL;
		}

		inline ~TinyBaseNode() {
			states.erase(m_state);
			Tiny_DestroyThread(&m_thread);
			Tiny_DeleteState(m_state);
		}

		inline bool init() {
			m_state = Tiny_CreateState();
			Tiny_InitThread(&m_thread, m_state);

			states.insert(m_state);
			return true;
		}

		inline bool hasFunction(std::string const& name) {
			return Tiny_GetFunctionIndex(m_state, name.c_str()) != -1;
		}

		template <typename T>
		inline void bindVariable(std::string const& name, std::string const& type, T val) {
			int idx = Tiny_BindGlobalVar(m_state, name.c_str(), type.c_str());
			Tiny_SetGlobal(&m_thread, idx, tiny_converter<T>::to(&m_thread, val));
		}

		template <TemplateStr name, typename F>
		void bindFunction(F fn) {
			_bindFunction<name>(typename extract_fn<F>::fn_type(fn));
		}

		inline void registerType(std::string const& name) {
			Tiny_RegisterType(m_state, name.c_str());
		}

		template <typename ...Args>
		inline void registerTypes(Args... names) {
			(registerType(names), ...);
		}

		inline Result<nullptr_t, std::string> compile(std::string const& contents) {
			auto out = Tiny_CompileString(m_state, typeid(*this).name(), contents.c_str());
			if (out.type == TINY_COMPILE_ERROR) {
				return Err("{}", out.error.msg);
			} else {
				return Ok(nullptr);
			}
		}

		template <typename R, typename ...Args>
		R runFunction(std::string const& name, Args... args) {
			int idx = Tiny_GetFunctionIndex(m_state, name.c_str());

			if (idx == -1) {
				log::error("Function '{}' not found", name);
				return R();
			}

			Tiny_Value cargs[] = {tiny_converter<Args>::to(&m_thread, args)...};
			return tiny_converter<R>::from(Tiny_CallFunction(&m_thread, idx, cargs, sizeof...(Args)));
		}
	};
}

using tiny_geode::TinyBaseNode;
