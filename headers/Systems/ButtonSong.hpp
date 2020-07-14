/*
** EPITECH PROJECT, 2020
** ButtonSong.hpp
** File description:
** Render
*/

#include "../ECS.hpp"
#include "../Components.hpp"
#include "../GameManager.hpp"

extern GameManager GM;

class ButtonSong : public ISystems {
    public:
        ButtonSong(){};
        void update() {
            for (Entity ID = 0; ID < MAX_ENTITIES; ID++) {
                if (GM.getComponent(ID, 4).bit != 0) {
                    Button &btn = dynamic_cast<Button&>(GM.getComponent(ID, 4));
                    Device &dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
                    if (btn.env->getHovered() == btn.btn && btn.musicHover) {
                        dvc.device->getCursorControl()->setActiveIcon(ECI_HAND);
                        if (btn.isPlayingHover == false) {
                            btn.isPlayingHover = true;
                            btn.musicHover->play();
                        }
                    } else {
                        btn.isPlayingHover = false;
                    }
                    if (btn.btn->isPressed() && btn.musicClick) {
                        if (btn.isPlayingClick == false) {
                            btn.isPlayingClick = true;
                            btn.musicClick->play();
                        }
                    } else
                            btn.isPlayingClick = false;
                }
            }
        };
        ~ButtonSong(){};
};