#include "../ImageItems/SaveSystem.hpp"
#include "CCSelectImagePopup.hpp"
#include "CCNode/CCImageSelectNode.hpp"
#include "../CCBannerItem.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
CCSelectImagePopup *CCSelectImagePopup::create(CCBannerItem *AttachedBanner)
{
    auto ret = new CCSelectImagePopup;

    if (ret && ret->pre_setup(AttachedBanner) && ret->initAnchored(420.f, 270.f, "GJ_square01.png"))
    {
        ret->autorelease();
        ret->setID("ImagePopup"_spr);
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
};

bool CCSelectImagePopup::pre_setup(CCBannerItem *AttachedBanner)
{
    m_BannerItem = AttachedBanner;
    return m_BannerItem != nullptr;
};

// i didn't want to remake searching lol
void CCSelectImagePopup::SortList(const std::string &query)
{
    auto NodeScroller = m_content->m_contentLayer;
    CCArrayExt<CCNode *> objects = NodeScroller->getChildren();
    shownCount = 0;
    for (auto *obj : objects)
    {
        if (CCImageSelectNode *Cell = typeinfo_cast<CCImageSelectNode *>(obj))
        {
            std::string name = Cell->getName();
            if (name.size() >= query.size())
            {
                bool matches = std::equal(query.begin(), query.end(), name.begin(),
                                          [](char a, char b)
                                          {
                                              return std::tolower(a) == std::tolower(b);
                                          });
                obj->setVisible(matches);
            }
            else
            {
                obj->setVisible(false);
            }
        }
        if (obj->isVisible())
            shownCount += 1;
    }
    Premove();
    m_content->moveToTop();
}
void CCSelectImagePopup::Premove()
{
    int i = -1;
    auto NodeScroller = m_content->m_contentLayer;
    CCArrayExt<CCNode *> objects = NodeScroller->getChildren();
    double height = std::max<double>(m_minsize, 40 * shownCount);
    for (auto *obj : objects)
    {
        if (obj->isVisible())
        {
            i++;
            obj->setPositionY(height - (40 * i));
        }
        else
        {
            obj->setPositionY(-10000);
        }
    }
    NodeScroller->setContentSize(ccp(NodeScroller->getContentSize().width, height));
}
bool CCSelectImagePopup::setup()
{
    auto winSize = m_mainLayer->getContentSize();
    m_content = ScrollLayer::create(ccp(
        m_mainLayer->getContentSize().width - 30,
        m_mainLayer->getContentSize().height - 70));
    m_content->setAnchorPoint(ccp(0, 0));
    m_content->setPosition(ccp(15, 15));
    m_mainLayer->addChild(m_content);
    m_minsize = m_content->m_contentLayer->getContentSize().height;
    auto searchBarBg = CCLayerColor::create(ccc4(0, 0, 0, 100), 210.f, 35.f);
    searchBarBg->setAnchorPoint(ccp(0.5f, 1));
    searchBarBg->setPosition(ccp(winSize.width / 2, winSize.height - 50));
    m_mainLayer->addChild(searchBarBg, -1);
    m_searchBar = TextInput::create(
        winSize.width / 2, "Search...", "bigFont.fnt");
    m_searchBar->setCallback([=](std::string input)
                             { SortList(input); });
    m_searchBar->setAnchorPoint(ccp(0.5f, 1));
    m_searchBar->setPosition(ccp(winSize.width / 2, winSize.height - 15));
    m_mainLayer->addChild(m_searchBar);

    auto plus_spr = CCSprite::createWithSpriteFrameName("GJ_plus2Btn_001.png");
    plus_spr->setScale(0.9);
    auto plusBtn = CCMenuItemSpriteExtra::create(
        plus_spr, this, menu_selector(CCSelectImagePopup::OpenPopup));
    plusBtn->setPosition(winSize.width, 0);

    auto divider = CCLayerColor::create(ccc4(200, 200, 200, 100), winSize.width - 30, 2);
    divider->setAnchorPoint(ccp(0, 1));
    divider->setPosition(ccp(15, winSize.height - 50));
    m_mainLayer->addChild(divider);
    m_buttonMenu->addChild(plusBtn);

    reload();
    return true;
}
void CCSelectImagePopup::OpenPopup(CCObject *Self)
{
    file::FilePickOptions::Filter textFilter;
    file::FilePickOptions fileOptions;
    textFilter.description = "Image files";
    textFilter.files = {"*.png", "*.jpeg"};
    fileOptions.filters.push_back(textFilter);
    file::pick(file::PickMode::OpenFile, {Mod::get()->getResourcesDir(), {textFilter}}).listen([=](Result<std::filesystem::path> *res)
                                                                                               {
    if (res->isOk()) {
        std::filesystem::path path = res->unwrap();
        matjson::Value response = SaveSystem_class->loadfile("SavedImages");
        std::string item = path.string();
        if (response.get("Paths"))
        {
        auto& p = response["Paths"].asArray().unwrap();

        bool exists = std::any_of(p.begin(), p.end(), [&](const auto& v) {
            return v.asString().unwrap() == item;
        });

        if (!exists)
        {
            p.push_back(item);
            SaveSystem_class->writefile("SavedImages", response);
            reload();
        }
    } else {
        auto noItems = matjson::makeObject({std::make_pair("Paths", matjson::Value(std::vector<matjson::Value>{
															matjson::Value(item)}))});
       SaveSystem_class->writefile("SavedImages", noItems);
       reload();
    }
    } });
}
CCImageSelectNode *CCSelectImagePopup::itemNew(matjson::Value item)
{
    bool showHelp = false;
    if (this->m_BannerItem)
    {
        showHelp = alreadyitemshowninghelp != true && this->m_BannerItem->ShowUserHow;
        alreadyitemshowninghelp = alreadyitemshowninghelp || showHelp;
    }
    CCImageSelectNode *itemnode = CCImageSelectNode::create(item);
    itemnode->selectionobject = [=]
    {
        if (this->m_BannerItem->ShowUserHow)
        {
            this->m_BannerItem->ShowUserHow = false;

            if (this->m_BannerItem->m_Overlay)
            {
                this->m_BannerItem->m_Overlay->removeFromParentAndCleanup(true);
            }
            if (this->m_BannerItem->arrowref)
            {
                this->m_BannerItem->arrowref->removeFromParentAndCleanup(true);
            }
        }
        // log::debug("should load {}",itemnode->m_filePath);
        this->m_BannerItem->save(itemnode->m_filePath);
        this->m_BannerItem->changeimage(itemnode->m_filePath);
        geode::Popup<>::onClose(nullptr);
    };
    if (showHelp)
    {
        showHelp = false;
        itemnode->CreateArrow();
    }
    m_content->m_contentLayer->addChild(itemnode);
    return itemnode;
}
void CCSelectImagePopup::reload()
{
    m_content->m_contentLayer->removeAllChildren();

    for (const auto item : SaveSystem_class->DefaultPaths.asArray().unwrap())
    {
        itemNew(item)->m_Icon->removeFromParentAndCleanup(true);
    }
    // screams in array
    matjson::Value response = SaveSystem_class->loadfile("SavedImages");
    if (response.get("Paths"))
    {
        const matjson::Value paths = response["Paths"].asArray().unwrap();
        for (auto item : paths)
        {
            CCImageSelectNode *itemNode = itemNew(item);
            itemNode->m_garbcan = [=]
            {
                matjson::Value response = SaveSystem_class->loadfile("SavedImages");
                auto &p = response["Paths"].asArray().unwrap();

                auto it = std::find(p.begin(), p.end(), item);
                if (it != p.end())
                {
                    p.erase(it);
                    SaveSystem_class->writefile("SavedImages", response);
                }
                itemNode->removeFromParentAndCleanup(true);
                reload();
            };
        }
    }
    SortList(m_searchBar->getString());
}
