#pragma once
#include <Geode/Geode.hpp>
#include "../CCBannerItem.hpp"
#include "CCNode/CCImageSelectNode.hpp"
using namespace geode::prelude;

class CCSelectImagePopup : public geode::Popup<> {
    protected:
            double m_minsize; 
            int shownCount;
            CCBannerItem* m_BannerItem;
            bool alreadyitemshowninghelp = false;
    public:
        TextInput* m_searchBar;
        ScrollLayer* m_content;
        static CCSelectImagePopup* create(CCBannerItem* AttachedBanner);
        CCImageSelectNode* itemNew(matjson::Value item);
        bool pre_setup(CCBannerItem* AttachedBanner);
        bool setup();
        void OpenPopup(CCObject* x);
        void SortList(const std::string& query);
        void Premove();
        void reload();
};
