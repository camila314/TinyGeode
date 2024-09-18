#include "Geode/cocos/platform/CCImage.h"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <TinyNode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		Build(this).schedule([=](float dt) {
			auto file = std::filesystem::path(__FILE__).parent_path() / "example.tiny";

			if (!std::filesystem::exists(file))
				return;

			auto last_write = std::filesystem::last_write_time(file);
			static decltype(last_write) last_check;

			if (last_check != last_write) {
				last_check = last_write;

				std::string code = utils::file::readString(file).unwrap();

				if (auto node = TinyNode::create(code)) {
					this->removeChildByID("tiny-node");
					node->setID("tiny-node");
					node->setZOrder(99999);
					Build(node).parent(this).center();
				}
			}
		});

		return true;
	}
};
