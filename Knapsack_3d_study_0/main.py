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


cont_x = 120
cont_y = 60
cont_z = 80

def create_container():
    left_wall = Entity(model = "cube", color = rgb(0,0,0,1), scale = 1, collider = "box")
    left_wall.position = (-cont_x/2,0,0)
    left_wall.scale_x = 0.01
    left_wall.scale_y = cont_y
    left_wall.scale_z = cont_z

    line1 = Entity(model = "cube", color = rgb(255,255,255), scale = 1, collider = "box")
    line1.position = (-cont_x/2 + 0.01,-cont_y/2 + 0.01,0)
    line1.scale_x = 0.01
    line1.scale_y = 0.01
    line1.scale_z = cont_z



    bottom_wall = Entity(model = "cube", color = rgb(0,0,0,1), scale = 1, collider = "box")
    bottom_wall.position = (0,-cont_y/2,0)
    bottom_wall.scale_x = cont_x
    bottom_wall.scale_y = 0.01
    bottom_wall.scale_z = cont_z

    line2 = Entity(model="cube", color=rgb(255, 255, 255), scale=1, collider="box")
    line2.position = (0, -cont_y / 2 + 0.01, cont_z/2 - 0.01)
    line2.scale_x = cont_x
    line2.scale_y = 0.01
    line2.scale_z = 0.01



    behind_wall = Entity(model="cube", color=rgb(0, 0, 0, 1), scale=1, collider="box")
    behind_wall.position = (0, 0, cont_z/2)
    behind_wall.scale_x = cont_x
    behind_wall.scale_y = cont_y
    behind_wall.scale_z = 0.01

    line3 = Entity(model="cube", color=rgb(255, 255, 255), scale=1, collider="box")
    line3.position = (-cont_x / 2 + 0.01, 0, cont_z/2 - 0.01)
    line3.scale_x = 0.01
    line3.scale_y = cont_y
    line3.scale_z = 0.01
    


def place_box(x0,y0,z0, x_len, y_len, z_len):
    i = x0 + 5
    j = y0 + 5
    k = z0 + 5
    box = Entity(model='cube', color=hsv((10*x0 + 5) % 256,
                                         (10*y0 + 5) % 256,
                                         (10*z0 + 5) % 256), scale=1, collider='box')
    box.position = (0 - cont_x/2 + x0 + x_len/2,
                    0 - cont_y/2 + y0 + y_len/2,
                    0 + cont_z/2 - z0 - z_len/2)

def do_boxes_intersect(x0_a, y0_a, z0_a, xlen_a, ylen_a, zlen_a,
                       x0_b, y0_b, z0_b, xlen_b, ylen_b, zlen_b):
    #intersection on x axis
    if(x0_a + xlen_a > x0_b or x0_b + xlen_b > x0_a):
        return True
    #intersection on y axis
    if (y0_a + ylen_a > y0_b or y0_b + ylen_b > y0_a):
        return True
    #intersection on z axis
    if (z0_a + zlen_a > z0_b or z0_b + zlen_b > z0_a):
        return True
    return False


#boxes placed, tuple (x,y,z), point (x0,y0,z0)
def placeable(placed_array, lengths, point):
    #does the box we want to place intersect with any other box?
    for box in placed_array:
        if do_boxes_intersect(box.x0, box.y0, box.z0, box.xlen, box.ylen, box.zlen,
                point.x, point.y, point.z, lengths(0), lengths(1), lengths(2)):
            return False
    #does the box we want to place exceed the container limitations?
    if(point.x + lengths(0) > cont_x or
        point.y + lengths(1) > cont_y or
        point.z + lengths(2) > cont_z):
            return False

    return True

class Box:
    def __init__(self, name, xlen, ylen, zlen):
        self.name = name
        self.xlen = xlen
        self.ylen = ylen
        self.zlen = zlen
        self.x0 = None
        self.y0 = None
        self.z0 = None

class Point:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

def give_permutation(x,y,z,i):
    #function to give the i-th permutation of the given values.
    #We could also use a generator, but since the permutations are
    #just six, in order to be more clear we'll list them explicitly
    if i == 1:
        return (x,y,z)
    if i == 2:
        return (x,z,y)
    if i == 3:
        return (y,x,z)
    if i == 4:
        return (y,z,x)
    if i == 5:
        return (z,x,y)
    if i == 6:
        return (z,y,x)


def start_ordering(boxes_array):
    unplaced = boxes_array[:]
    placed = []
    #the available points must be interpreted as the left_bottom_behind points available
    available_points = [Point(0,0,0)]
    order_boxes(unplaced, placed, available_points)






def order_boxes(unplaced, placed, available_points):
    if len(unplaced) == 0:
        return placed

    curr = unplaced[0]
    for possible_point in available_points: #for all the points available...
        for i in range(1,7):    #for all possible permutations of a box...
            perm = give_permutation(curr.xlen, curr.ylen, curr.zlen, i)
            if placeable(placed, perm, possible_point) == True: #if the box doesn't collide with any other box previously placed...



if __name__ == '__main__':
    app = Ursina()

    x = 1
    y = 1
    z = 1
    create_container()

    box1 = Box("Pippo", 20, 10, 30)
    box2 = Box("Pluto", 40, 30, 20)
    box3 = Box("Pietro", 30, 50, 60)

    boxes = [box1, box2, box3]

    start_ordering(boxes)


    cube1 = Entity(model='cube', color=hsv(200, 1, 1), scale=1, collider='box')

    place_box(0,0,0,x,y,z)
    place_box(1, 0, 0, x, y, z)




    editor_camera = EditorCamera(enabled = True)  # add camera controls for orbiting and moving the camera

    app.run()
