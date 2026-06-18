#ifndef GUI_CHILD_COMPONENT_H
#define GUI_CHILD_COMPONENT_H
#include <memory> 
#include <vector> 
#include <filesystem>
#include <iostream>



namespace Graphics {

    class GUIChildComponent
    {
        public: 
            GUIChildComponent(std::string name);
            virtual void draw() = 0;

        protected:
            std::string Name;
    };

    class ModelView : public GUIChildComponent
    {

        public: 
            ModelView(std::shared_ptr<std::vector<std::filesystem::directory_entry>> files)
            //ModelView()
            : GUIChildComponent("##ModelView" + childCount),
            files(files)
            {

                ++childCount;
            }
            void draw() override;

        private: 
            enum IconType
            {
                LINE,
                SMALL,
                MEDIUM, 
                LARGE
            };
            std::shared_ptr<std::vector<std::filesystem::directory_entry>> files;
            static std::vector<IconType> IconTypes;
            IconType IconSetting = MEDIUM;
            static int childCount;

    };
}






#endif