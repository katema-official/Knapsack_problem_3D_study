from ursina import *
from ursina import curve
from ursina.prefabs.first_person_controller import FirstPersonController


def camera_control():
    #camera.z += held_keys["w"] * 0.3
    #camera.z -= held_keys["s"] * 0.3
    camera.x += held_keys["d"] * 0.3
    camera.x -= held_keys["a"] * 0.3
    camera.y += held_keys["e"] * 0.3
    camera.y -= held_keys["q"] * 0.3


def update():
    camera_control()


if __name__ == '__main__':
    app = Ursina()

    cube1 = Entity(model='cube', color=hsv(200, 1, 1), scale=1, collider='box')
    cube1.position = (0,0,0)
    cube2 = Entity(model='cube', color=hsv(255, 1, 1), scale=0.5, collider='box')
    cube2.position = (1, 0, 0)
    cube3 = Entity(model='cube', color=hsv(280, 1, 1), scale=1, collider='box')
    cube3.position = (2, 0, 0)

    cube3 = Entity(model='cube', color=hsv(100, 1, 1), scale=1, collider='box')
    cube3.position = (0, 0, 1)



    editor_camera = EditorCamera(enabled = True)  # add camera controls for orbiting and moving the camera

    app.run()
