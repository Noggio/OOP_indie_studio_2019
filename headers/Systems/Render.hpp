/*
** EPITECH PROJECT, 2020
** Render.hpp
** File description:
** Render
*/

#include "../ECS.hpp"
#include "../Components.hpp"
#include "../GameManager.hpp"

extern GameManager GM;
extern double fixedDTime;
extern ITimer* pTimer;

class Render : public ISystems {
    public:
        Render () {
        };
        void update() {
                if (GM.getComponent(0, 3).bit != 0) {
                    Device &dvc = dynamic_cast<Device&>(GM.getComponent(0, 3));
                    if (dvc.device->run() && dvc.device->getVideoDriver() && dvc.device->isWindowActive()) {
                        dvc.device->getGUIEnvironment()->drawAll();
                        dvc.device->getSceneManager()->drawAll();
                        int fps = dvc.device->getVideoDriver()->getFPS();
                        stringw str = L"Indie Studio";
                        str += " FPS:";
                        str += fps;
                        dvc.device->setWindowCaption(str.c_str());
                        dvc.device->getVideoDriver()->endScene();
                    }
                }
        };
        ~Render() {
        };
};