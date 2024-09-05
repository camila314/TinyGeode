#include "TinyBaseNode.hpp"
#include <UIBuilder.hpp>

namespace tiny_geode {

	class TinyNode : public TinyBaseNode {
		CCMenu* m_menu;
	public:
		static TinyNode* create(std::string const& code) {
			auto ret = new TinyNode();
			if (ret && ret->init(code)) {
				ret->autorelease();
				return ret;
			}
			else {
				delete ret;
				return nullptr;
			}
		}

		inline void bindConstructors() {
			bindFunction<"Node(float, float): Node">([=](float x, float y) -> auto {
				auto node = CCNode::create();
				node->setPosition({x, y});
				this->addChild(node);
				return node;
			});

			bindFunction<"Label(float, float, str): Node">([=](float x, float y, std::string text) -> auto {
				auto lab = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
				lab->setPosition({x, y});
				this->addChild(lab);
				return lab;
			});

			bindFunction<"Sprite(float, float, str): Node">([=](float x, float y, std::string name) -> auto {
				auto spr = CCSprite::createWithSpriteFrameName(name.c_str());
				if (spr == nullptr)
					spr = CCSprite::create(name.c_str());
				if (spr == nullptr)
					spr = CCSprite::createWithSpriteFrameName("exMark_001.png");

				spr->setPosition({x, y});
				this->addChild(spr);

				return spr;
			});

			bindFunction<"Button(Node, str): Node">([=](CCSprite* sprite, std::string callback) -> auto {
				auto pos = sprite->getPosition();
				sprite->removeFromParent();

				auto btn = Build(sprite).intoMenuItem([=]() {
					this->runFunction<void>(callback);
				}).pos(pos).parent(m_menu).collect();

				return btn;
			});

			bindFunction<"Scale9Sprite(float, float, float, float, str): Node">([=](float x, float y, float w, float h, std::string name) -> auto {
				auto spr = CCScale9Sprite::create(name.c_str());
				if (spr == nullptr)
					spr = CCScale9Sprite::create("GJ_square01.png");
				spr->setContentSize({w, h});
				spr->setPosition({x, y});
				this->addChild(spr);

				return spr;
			});

		}

		inline void bindNodeFns() {
			bindFunction<"tag(Node, int): void">([=](CCNode* node, int tag) -> void {
				node->setTag(tag);
			});
			bindFunction<"fetch(int): Node">([=](int tag) -> CCNode* {
				if (auto t = this->getChildByTag(tag))
					return t;
				else
					return this->m_menu->getChildByTag(tag);
			});

			bindFunction<"visible(Node, bool): void">([=](CCNode* node, bool vis) -> void {
				node->setVisible(vis);
			});

			bindFunction<"pos(Node, float, float): void">([=](CCNode* node, float x, float y) -> void {
				node->setPosition({x, y});
			});
			bindFunction<"xpos(Node, float): void">([=](CCNode* node, float x) -> void {
				node->setPositionX(x);
			});	
			bindFunction<"ypos(Node, float): void">([=](CCNode* node, float y) -> void {
				node->setPositionY(y);
			});
			bindFunction<"move(Node, float, float): void">([=](CCNode* node, float x, float y) -> void {
				node->setPosition(node->getPosition() + ccp(x, y));
			});

			bindFunction<"scale(Node, float): void">([=](CCNode* node, float scale) -> void {
				node->setScale(scale);
			});
			bindFunction<"scaleBy(Node, float): void">([=](CCNode* node, float scale) -> void {
				node->setScale(node->getScale() * scale);
			});

			bindFunction<"rotate(Node, float): void">([=](CCNode* node, float angle) -> void {
				node->setRotation(angle);
			});
			bindFunction<"rotateBy(Node, float): void">([=](CCNode* node, float angle) -> void {
				node->setRotation(node->getRotation() + angle);
			});

			bindFunction<"z(Node, int): void">([=](CCNode* node, int z) -> void {
				node->setZOrder(z);
			});

			bindFunction<"child(Node, Node): void">([=](CCNode* parent, CCNode* child) -> void {
				child->removeFromParent();
				parent->addChild(child);
			});

			bindFunction<"runAction(Node, Action): void">([=](CCNode* node, CCAction* action) -> void {
				node->runAction(action);
			});

			bindFunction<"color(Node, int, int, int): void">([=](CCNode* node, uint8_t r, uint8_t g, uint8_t b) -> void {
				if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(node))
					rgba->setColor({r, g, b});
				else
					log::error("Node does not support color");
			});
			bindFunction<"opacity(Node, int): void">([=](CCNode* node, uint8_t op) -> void {
				if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(node))
					rgba->setOpacity(op);
				else
					log::error("Node does not support opacity");
			});
			bindFunction<"text(Node, str): void">([=](CCNode* node, std::string text) -> void {
				if (auto lab = typeinfo_cast<CCLabelBMFont*>(node))
					lab->setString(text.c_str());
				else
					log::error("Node does not support text");
			});
			bindFunction<"font(Node, str): void">([=](CCNode* node, std::string font) -> void {
				auto fu = CCFileUtils::sharedFileUtils();

				if (!fu->isFileExist(fu->fullPathForFilename(font.c_str(), false))) {
					log::error("Invalid font file {}", font);
					font = "bigFont.fnt";
				}

				if (auto lab = typeinfo_cast<CCLabelBMFont*>(node))
					lab->setFntFile(font.c_str());
				else
					log::error("Node does not support font");
			});
		}

		inline void bindActionConstructors() {
			bindFunction<"Sequence(Action, Action): Action">([=](CCFiniteTimeAction* action1, CCFiniteTimeAction* action2) -> CCAction* {
				return CCSequence::create(action1, action2);
			});

			bindFunction<"Delay(float): Action">([=](float time) -> CCAction* {
				return CCDelayTime::create(time);
			});

			bindFunction<"MoveTo(float, float, float): Action">([=](float time, float x, float y) -> CCAction* {
				return CCMoveTo::create(time, {x, y});
			});
			bindFunction<"MoveBy(float, float, float): Action">([=](float time, float x, float y) -> CCAction* {
				return CCMoveBy::create(time, {x, y});
			});

			bindFunction<"ScaleTo(float, float): Action">([=](float time, float scale) -> CCAction* {
				return CCScaleTo::create(time, scale);
			});
			bindFunction<"ScaleBy(float, float): Action">([=](float time, float scale) -> CCAction* {
				return CCScaleBy::create(time, scale);
			});

			bindFunction<"RotateTo(float, float): Action">([=](float time, float angle) -> CCAction* {
				return CCRotateTo::create(time, angle);
			});
			bindFunction<"RotateBy(float, float): Action">([=](float time, float angle) -> CCAction* {
				return CCRotateBy::create(time, angle);
			});

			bindFunction<"FadeTo(float, float): Action">([=](float time, float opacity) -> CCAction* {
				return CCFadeTo::create(time, opacity);
			});
			bindFunction<"FadeBy(float, float): Action">([=](float time, float opacity) -> CCAction* {
				return CCFadeTo::create(time, opacity);
			});
		}

		inline void bindSpecial() {
			bindFunction<"popup(str, str, str): void">([=](std::string title, std::string message, std::string button) -> void {
				FLAlertLayer::create(title.c_str(), message, button.c_str())->show();
			});

			bindFunction<"prompt(str, str, str): void">([=](std::string title, std::string placeholder, std::string callback) -> void {
				auto alert = new FLAlertLayer();
				alert->init(150);
				alert->autorelease();
				alert->m_buttonMenu = CCMenu::create();
				alert->m_buttonMenu->setZOrder(99999);
				alert->m_mainLayer->addChild(alert->m_buttonMenu);

				Build(alert->m_mainLayer)
					.intoNewChild(CCScale9Sprite::create("GJ_square01.png", CCRectZero))
						.contentSize(300, 150)
						.center()
					.intoNewSibling(CCLabelBMFont::create(title.c_str(), "bigFont.fnt"))
						.scale(0.8)
						.center().move(0, 45)
					.intoNewSibling(CCScale9Sprite::create("square02b_small.png", CCRectZero))
						.contentSize(220, 30)
						.color(0, 0, 0)
						.opacity(100)
						.center()
					.intoNewSibling(CCTextInputNode::create(400, 50, placeholder.c_str(), "Thonburi", 18, "bigFont.fnt"))
						.anchorPoint({0, 0})
						.scale(0.5)
						.placeholderColor(ccc3(200, 200, 200))
						.tag(1)
						.center();

				Build<ButtonSprite>::create("Ok")
					.intoMenuItem([=]() {
						this->runFunction<void>(callback, static_cast<CCTextInputNode*>(alert->m_mainLayer->getChildByTag(1))->getString());
						alert->removeFromParentAndCleanup(true);
					})
					.pos(0, -45)
					.parent(alert->m_buttonMenu);

				alert->show();
			});

			bindFunction<"terminate(): void">([=](CCNode* node) -> void {
				this->removeFromParentAndCleanup(true);
			});

			bindFunction<"self_menu(): Node">([=]() -> CCMenu* {
				return m_menu;
			});

			bindFunction<"self(): Node">([=]() -> CCNode* {
				return this;
			});
		}

		inline bool init(std::string const& code) {
			TinyBaseNode::init();
			m_menu = CCMenu::create();
			m_menu->setPosition({0, 0});
			this->addChild(m_menu);

			registerType("Node");
			registerType("Action");
			bindConstructors();
			bindActionConstructors();
			bindNodeFns();
			bindSpecial();

			if (auto err = compile(code).err()) {
				log::error("Error: {}", err.value());
				return false;
			} else {
				runFunction<void>("main");
				return true;
			}
		}
	};
}

using tiny_geode::TinyNode;
