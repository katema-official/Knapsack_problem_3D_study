import boxmodule_misc as bs
from ursina import *

#camera controls for ursina camera (can be changed)
def camera_control():
    #camera.z += held_keys["w"] * 0.3
    #camera.z -= held_keys["s"] * 0.3
    camera.x += held_keys["d"] * 0.3
    camera.x -= held_keys["a"] * 0.3
    camera.y += held_keys["e"] * 0.3
    camera.y -= held_keys["q"] * 0.3

#class with a method for scattering the boxes. It's used when the "p" key is pressed
class BoxesController:
    sorted_boxes = []
    state = -2

    def move_boxes(cls):
        cls.state = cls.state + 1
        print(cls.state)
        if(cls.state == -1):
            enlarge_boxes(cls.sorted_boxes)
        #else:
        #    if(cls.state < len(cls.sorted_boxes)):
        #        replace_box(cls.sorted_boxes[cls.state])
        #    else:
        #        cls.state = -2
        cls.state = -2

#see class above
def input(key):
    if key == "p":
        BoxesController.move_boxes(BoxesController)

def update():
    camera_control()


# function for creating the container (bin) that will contain the boxes.
# Note that this container has only the left, behind and bottom walls.
# This is done for allowing simpler visualization of the boxes inside of it.
def create_container():
    left_wall = Entity(model="cube", color=rgb(0, 0, 0, 1), scale=1, collider="box")
    left_wall.position = (-bs.cont_x / 2, 0, 0)
    left_wall.scale_x = 0.01
    left_wall.scale_y = bs.cont_y
    left_wall.scale_z = bs.cont_z

    line1 = Entity(model="cube", color=rgb(255, 255, 255), scale=1, collider="box")
    line1.position = (-bs.cont_x / 2 + 0.01, -bs.cont_y / 2 + 0.01, 0)
    line1.scale_x = 0.01
    line1.scale_y = 0.01
    line1.scale_z = bs.cont_z

    bottom_wall = Entity(model="cube", color=rgb(0, 0, 0, 1), scale=1, collider="box")
    bottom_wall.position = (0, -bs.cont_y / 2, 0)
    bottom_wall.scale_x = bs.cont_x
    bottom_wall.scale_y = 0.01
    bottom_wall.scale_z = bs.cont_z

    line2 = Entity(model="cube", color=rgb(255, 255, 255), scale=1, collider="box")
    line2.position = (0, -bs.cont_y / 2 + 0.01, bs.cont_z / 2 - 0.01)
    line2.scale_x = bs.cont_x
    line2.scale_y = 0.01
    line2.scale_z = 0.01

    behind_wall = Entity(model="cube", color=rgb(0, 0, 0, 1), scale=1, collider="box")
    behind_wall.position = (0, 0, bs.cont_z / 2)
    behind_wall.scale_x = bs.cont_x
    behind_wall.scale_y = bs.cont_y
    behind_wall.scale_z = 0.01

    line3 = Entity(model="cube", color=rgb(255, 255, 255), scale=1, collider="box")
    line3.position = (-bs.cont_x / 2 + 0.01, 0, bs.cont_z / 2 - 0.01)
    line3.scale_x = 0.01
    line3.scale_y = bs.cont_y
    line3.scale_z = 0.01


# function used to change the position of a box
def replace_box(box):
    box.entity.x = 0 - bs.cont_x / 2 + box.x0 + box.xlen / 2
    box.entity.y = 0 - bs.cont_y / 2 + box.y0 + box.ylen / 2
    box.entity.z = 0 + bs.cont_z / 2 - box.z0 - box.zlen / 2


# function that takes the placed boxes and scatters them
def enlarge_boxes(boxes):
    for box in boxes:
        # if box.x0 == box.z0: pass
        # else:
        if box.x0 > box.z0:
            box.entity.x = 0 - bs.cont_x / 2 + box.x0 + bs.cont_x + box.xlen
        else:
            box.entity.z = 0 + bs.cont_z / 2 - box.z0 - bs.cont_z - box.zlen
    # box.entity.y = 0 - cont_y / 2 + box.y0 + box.ylen * 2


# Function that, given a LBB point and the width, height and depth of a box,
# places a box with those dimensions with the specified LBB point.
def place_box(x0, y0, z0, x_len, y_len, z_len, color_box = None):
    color_box = color_box if color_box!=None else rgb(random.randint(0, 255),
                                                      random.randint(0, 255), random.randint(0, 255))
    box = Entity(model='cube', color=color_box, scale=1, collider='box')
    box.scale_x = x_len
    box.scale_y = y_len
    box.scale_z = z_len
    box.position = (0 - bs.cont_x / 2 + x0 + x_len / 2,
                    0 - bs.cont_y / 2 + y0 + y_len / 2,
                    0 + bs.cont_z / 2 - z0 - z_len / 2)

    return box




#Class that represents a box to be packed.
class Box:
    def __init__(self, name, xlen, ylen, zlen, color=None):
        self.name = name    #identifier of the box
        self.xlen = xlen    #its length along the x axis
        self.ylen = ylen    #its length along the y axis
        self.zlen = zlen    #its length along the z axis
        self.x0 = None      #the x cordinate of its LBB point
        self.y0 = None      #the y cordinate of its LBB point
        self.z0 = None      #the z cordinate of its LBB point
        self.entity = None  #The entity associated with this box. It's the actual box basically (of ursina)
        self.color = color   #color to be used for this boxed. If Node, will be chosen randomly

#Class to represent a point
class Point:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

#function for generating randomly a box
def randomly_generate_box(max_x, max_y, max_z):
    box = Box("a", random.randint(10,max_x), random.randint(10,max_y), random.randint(10,max_z))
    return box


