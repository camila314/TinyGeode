#include "TinyBaseNode.hpp"
#include <UIBuilder.hpp>

namespace tiny_geode {

	class TinyNode : public TinyBaseNode {
		CCMenu* m_menu;
		std::unordered_map<std::string, CCNode*> m_nodeMap;
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

		inline void bindGlobal(std::string const& name, CCNode* n) {
			m_nodeMap[name] = n;
		}

		inline void bindConstructors() {
			bindFunction<"Node(): Node">([=]() -> auto {
				auto node = CCNode::create();
				this->addChild(node);
				return node;
			});

			bindFunction<"Label(str, str): Node">([=](std::string text, std::string font) -> auto {
				auto lab = CCLabelBMFont::create(text.c_str(), font.c_str());
				this->addChild(lab);
				return lab;
			});

			bindFunction<"Sprite(str): Node">([=](std::string name) -> auto {
				auto spr = CCSprite::createWithSpriteFrameName(name.c_str());
				if (spr == nullptr)
					spr = CCSprite::create(name.c_str());
				if (spr == nullptr)
					spr = CCSprite::createWithSpriteFrameName("exMark_001.png");

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

			bindFunction<"ButtonSprite(str, str): Node">([=](std::string name, std::string sprite) -> auto {
				auto lbl = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
				lbl->setScale(0.7);
				auto spr = extension::CCScale9Sprite::create(sprite.c_str(), CCRectZero);
				spr->setContentSize(lbl->getContentSize());
				spr->addChild(lbl);
				lbl->setPosition(spr->getContentSize() / 2);

				return spr;
			});

			bindFunction<"Scale9Sprite(float, float, str): Node">([=](float w, float h, std::string name) -> auto {
				auto spr = extension::CCScale9Sprite::create(name.c_str(), CCRectZero);
				if (spr == nullptr)
					spr = extension::CCScale9Sprite::create("GJ_square01.png", CCRectZero);
				spr->setContentSize({w, h});
				this->addChild(spr);

				return spr;
			});

		}

		inline void bindNodeFns() {
			bindFunction<"tag(Node, int): Node">([=](CCNode* node, int tag) -> CCNode* {
				node->setTag(tag);
				return node;
			});
			bindFunction<"fetch(Node, int): Node">([=](CCNode* node, int tag) -> CCNode* {
				return node->getChildByTag(tag);
			});

			bindFunction<"visible(Node, bool): Node">([=](CCNode* node, bool vis) -> CCNode* {
				node->setVisible(vis);
				return node;
			});

			bindFunction<"pos(Node, float, float): Node">([=](CCNode* node, float x, float y) -> CCNode* {
				node->setPosition({x, y});
				return node;
			});
			bindFunction<"xpos(Node, float): Node">([=](CCNode* node, float x) -> CCNode* {
				node->setPositionX(x);
				return node;
			});	
			bindFunction<"ypos(Node, float): Node">([=](CCNode* node, float y) -> CCNode* {
				node->setPositionY(y);
				return node;
			});
			bindFunction<"getX(Node): float">([=](CCNode* node) -> float {
				return node->getPositionX();
			});
			bindFunction<"getY(Node): float">([=](CCNode* node) -> float {
				return node->getPositionY();
			});
			bindFunction<"move(Node, float, float): Node">([=](CCNode* node, float x, float y) -> CCNode* {
				node->setPosition(node->getPosition() + ccp(x, y));
				return node;
			});

			bindFunction<"size(Node, float, float): Node">([=](CCNode* node, float w, float h) -> CCNode* {
				node->setContentSize({w, h});
				return node;
			});
			bindFunction<"width(Node, float): Node">([=](CCNode* node, float w) -> CCNode* {
				node->setContentSize({w, node->getContentSize().height});
				return node;
			});
			bindFunction<"height(Node, float): Node">([=](CCNode* node, float h) -> CCNode* {
				node->setContentSize({node->getContentSize().width, h});
				return node;
			});
			bindFunction<"getWidth(Node): float">([=](CCNode* node) -> float {
				return node->getContentSize().width;
			});
			bindFunction<"getHeight(Node): float">([=](CCNode* node) -> float {
				return node->getContentSize().height;
			});

			bindFunction<"scale(Node, float): Node">([=](CCNode* node, float scale) -> CCNode* {
				node->setScale(scale);
				return node;
			});
			bindFunction<"scaleX(Node, float): Node">([=](CCNode* node, float scale) -> CCNode* {
				node->setScaleX(scale);
				return node;
			});
			bindFunction<"scaleY(Node, float): Node">([=](CCNode* node, float scale) -> CCNode* {
				node->setScaleY(scale);
				return node;
			});
			bindFunction<"scaleBy(Node, float): Node">([=](CCNode* node, float scale) -> CCNode* {
				node->setScale(node->getScale() * scale);
				return node;
			});

			bindFunction<"rotate(Node, float): Node">([=](CCNode* node, float angle) -> CCNode* {
				node->setRotation(angle);
				return node;
			});
			bindFunction<"rotateBy(Node, float): Node">([=](CCNode* node, float angle) -> CCNode* {
				node->setRotation(node->getRotation() + angle);
				return node;
			});

			bindFunction<"z(Node, int): Node">([=](CCNode* node, int z) -> CCNode* {
				node->setZOrder(z);
				return node;
			});
			bindFunction<"getZ(Node): int">([=](CCNode* node) -> int {
				return node->getZOrder();
			});

			bindFunction<"child(Node, Node): Node">([=](CCNode* parent, CCNode* child) -> CCNode* {
				child->removeFromParent();
				parent->addChild(child);
				return parent;
			});

			bindFunction<"runAction(Node, Action): Node">([=](CCNode* node, CCAction* action) -> CCNode* {
				node->runAction(action);
				return node;
			});

			bindFunction<"color(Node, int, int, int): Node">([=](CCNode* node, uint8_t r, uint8_t g, uint8_t b) -> CCNode* {
				if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(node))
					rgba->setColor({r, g, b});
				else
					log::error("Node does not support color");

				return node;
			});
			bindFunction<"opacity(Node, int): void">([=](CCNode* node, uint8_t op) -> CCNode* {
				if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(node))
					rgba->setOpacity(op);
				else
					log::error("Node does not support opacity");

				return node;
			});
			bindFunction<"text(Node, str): Node">([=](CCNode* node, std::string text) -> CCNode* {
				if (auto lab = typeinfo_cast<CCLabelBMFont*>(node))
					lab->setString(text.c_str());
				else
					log::error("Node does not support text");

				return node;
			});
			bindFunction<"font(Node, str): Node">([=](CCNode* node, std::string font) -> CCNode* {
				auto fu = CCFileUtils::sharedFileUtils();

				if (!fu->isFileExist(fu->fullPathForFilename(font.c_str(), false))) {
					log::error("Invalid font file {}", font);
					font = "bigFont.fnt";
				}

				if (auto lab = typeinfo_cast<CCLabelBMFont*>(node))
					lab->setFntFile(font.c_str());
				else
					log::error("Node does not support font");

				return node;
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

			bindFunction<"getGlobal(str): Node">([=](std::string name) {
				return m_nodeMap[name];
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
				if (hasFunction("main"))
					runFunction<void>("main");
				return true;
			}
		}
	};
}

using tiny_geode::TinyNode;
