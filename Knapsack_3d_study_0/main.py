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

def input(key):
    if key == "p":
        BoxesController.move_boxes(BoxesController)

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
    


def replace_box(box):
    box.entity.x = 0 - cont_x / 2 + box.x0 + box.xlen / 2
    box.entity.y = 0 - cont_y / 2 + box.y0 + box.ylen / 2
    box.entity.z = 0 + cont_z / 2 - box.z0 - box.zlen / 2

def enlarge_boxes(boxes):
    for box in boxes:
        #if box.x0 == box.z0: pass
        #else:
            if box.x0 > box.z0:
                box.entity.x = 0 - cont_x/2 + box.x0 + cont_x + box.xlen
            else:
                box.entity.z = 0 + cont_z/2 - box.z0 - cont_z - box.zlen
        #box.entity.y = 0 - cont_y / 2 + box.y0 + box.ylen * 2


def place_box(x0,y0,z0, x_len, y_len, z_len):
    box = Entity(model='cube', color=rgb(random.randint(0,255),
                                         random.randint(0,255),
                                         random.randint(0,255)), scale=1, collider='box')
    box.scale_x = x_len
    box.scale_y = y_len
    box.scale_z = z_len
    box.position = (0 - cont_x/2 + x0 + x_len/2,
                    0 - cont_y/2 + y0 + y_len/2,
                    0 + cont_z/2 - z0 - z_len/2)

    return box

def do_boxes_intersect(x0_a, y0_a, z0_a, xlen_a, ylen_a, zlen_a,
                       x0_b, y0_b, z0_b, xlen_b, ylen_b, zlen_b):
    #intersection on x axis
    overlapx = False
    if(x0_a < x0_b):
        overlapx = x0_a + xlen_a > x0_b
    else:
        if(x0_a == x0_b): overlapx = True
        else: overlapx = x0_b + xlen_b > x0_a

    #intersection on y axis
    overlapy = False
    if (y0_a < y0_b):
        overlapy = y0_a + ylen_a > y0_b
    else:
        if (y0_a == y0_b): overlapy = True
        else: overlapy = y0_b + ylen_b > y0_a

    #intersection on z axis
    overlapz = False
    if (z0_a < z0_b):
        overlapz = z0_a + zlen_a > z0_b
    else:
        if (z0_a == z0_b): overlapz = True
        else: overlapz = z0_b + zlen_b > z0_a

    if(overlapx and overlapy and overlapz): return True

    return False


#boxes placed, tuple (x,y,z), point (x0,y0,z0)
def placeable(placed_array, lengths, point):

    #does the box we want to place exceed the container limitations?
    if (point.x + lengths[0] > cont_x or
            point.y + lengths[1] > cont_y or
            point.z + lengths[2] > cont_z):
        return False

    #does the box we want to place intersect with any other box?
    for box in placed_array:
        if do_boxes_intersect(box.x0, box.y0, box.z0, box.xlen, box.ylen, box.zlen,
                point.x, point.y, point.z, lengths[0], lengths[1], lengths[2]):
            return False

    return True






def base_area(left_z, right_z, left_x, right_x, base_box):
    true_left_z = max(left_z, base_box.z0)
    true_right_z = min(right_z, base_box.z0 + base_box.zlen)
    true_left_x = max(left_x, base_box.x0)
    true_right_x = min(right_x, base_box.x0 + base_box.xlen)
    if true_right_z > true_left_z and true_right_x > true_left_x:
        return (true_right_z - true_left_z) * (true_right_x - true_left_x)
    return 0

#to check if "box" has a base to be placed, in order for it to not fall
def does_box_have_base(box, point, placed_boxes):
    if point.y == 0:
        return True

    #firs: find the boxes that could constitute the base of the new box
    boxes_with_correct_height = []
    for b in placed_boxes:
        if b.y0 + b.ylen == point.y:    #if the upper surface of the box could be a base for the new point...
            boxes_with_correct_height.append(b)

    #second: compute the total area that can sustain the new box
    areas = []
    for b in boxes_with_correct_height:
        a = base_area(point.z, point.z+box.zlen, point.x, point.x+box.xlen, b)
        if a > 0:
            areas.append(a)
    final_base_area = sum(areas)

    #third: check if the box has a base large enough to be sustained
    if final_base_area >= (box.zlen * box.xlen) / 2:
        return True

    return False







class Box:
    def __init__(self, name, xlen, ylen, zlen):
        self.name = name
        self.xlen = xlen
        self.ylen = ylen
        self.zlen = zlen
        self.x0 = None
        self.y0 = None
        self.z0 = None
        self.entity = None

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


def start_ordering(boxes_array, solutions):
    unplaced = boxes_array[:]
    placed = []
    #the available points must be interpreted as the left_bottom_behind points available
    available_points = [Point(0,0,0)]
    order_boxes(unplaced, placed, available_points, solutions)






def order_boxes(unplaced, placed, available_points, solutions):

    if(len(solutions) == 0):

        if len(unplaced) == 0:
            solutions.append(placed)
            for elem in solutions[0]:
                print("{}, {}, {}, {}, {}, {}\n".format(elem.x0, elem.y0, elem.z0, elem.xlen, elem.ylen, elem.zlen))

        else:
            for box_to_place in unplaced:   #for all the boxes that still need to be placed...
                for possible_point in available_points: #for all the points available...
                    for i in range(1,7):    #for all possible permutations of a box...
                        perm = give_permutation(box_to_place.xlen, box_to_place.ylen, box_to_place.zlen, i)
                        if does_box_have_base(box_to_place, possible_point, placed):    #EXTRA control: if the box wouldn't float mid-air...
                            if placeable(placed, perm, possible_point): #if the box doesn't collide with any other box previously placed...
                                #we can place the box
                                box_copy = Box(box_to_place.name, perm[0], perm[1], perm[2])
                                box_copy.x0 = possible_point.x
                                box_copy.y0 = possible_point.y
                                box_copy.z0 = possible_point.z
                                #box_to_place.xlen = perm[0]
                                #box_to_place.ylen = perm[1]
                                #box_to_place.zlen = perm[2]
                                new_available_points = available_points[:]
                                new_available_points.remove(possible_point)
                                new_available_points.extend([Point(box_copy.x0 + box_copy.xlen,
                                                                  box_copy.y0,
                                                                  box_copy.z0),
                                                    Point(box_copy.x0,
                                                                  box_copy.y0 + box_copy.ylen,
                                                                  box_copy.z0),
                                                    Point(box_copy.x0,
                                                                  box_copy.y0,
                                                                  box_copy.z0 + box_copy.zlen)])
                                new_unplaced = unplaced[:]
                                new_unplaced.remove(box_to_place)
                                new_placed = placed[:]
                                new_placed.append(box_copy)
                                order_boxes(new_unplaced,
                                            new_placed,
                                            new_available_points,
                                            solutions)


def randomly_generate_box(max_x, max_y, max_z):
    box = Box("a", random.randint(10,max_x), random.randint(10,max_y), random.randint(10,max_z))
    return box


if __name__ == '__main__':
    app = Ursina()

    x = 1
    y = 1
    z = 1
    create_container()

    box1 = Box("Pippo", 20, 10, 30)
    box2 = Box("Pluto", 40, 30, 20)
    box3 = Box("Pietro", 30, 50, 60)
    box4 = Box("Paperino", 50, 30, 80)
    box5 = Box("Topolino", 30, 40, 10)
    box6 = Box("Minnie", 50, 20, 70)
    box7 = Box("Aaa", 70, 40, 60)
    box8 = Box("Bbb", 30, 10, 20)

    boxes = [box1, box2, box3, box4, box5, box6, box7, box8]

    #boxes = []
    #for i in range(10):
    #    boxes.append(randomly_generate_box(50,50,50))

    start = time.time()

    solutions = []
    start_ordering(boxes, solutions)
    print("solutions are " + str(len(solutions)))
    for elem in solutions[0]:
        print("{}, {}, {}, {}, {}, {}\n".format(elem.x0, elem.y0, elem.z0, elem.xlen, elem.ylen, elem.zlen))


    final_solution = solutions[0]
    for sol in solutions[1:]:
        if len(sol) > len(final_solution):
            final_solution = sol
            print("updated")

    #different ways to sort the boxes
    #let's sort the boxes from the closest one to (0,0,0) to the furthest
    final_solution.sort(reverse = True, key = lambda box: (0 - cont_x / 2 + box.x0)**2 +
                                                            (0 - cont_y / 2 + box.y0)**2 +
                                                            (0 + cont_z / 2 - box.z0)**2 )

    #let's sort starting from the ones below
    #final_solution.sort(reverse=False, key=lambda box: (0 - cont_y + box.y0)
    #                                                    + (0 - cont_x / 2 + box.x0)**2
    #                                                    + (0 + cont_z / 2 - box.z0)**2)



    for b in final_solution:
        b.entity = place_box(b.x0, b.y0, b.z0, b.xlen, b.ylen, b.zlen)


        def tmp(box):
            def replace():
                print("hi")
                box.entity.x = 0 - cont_x / 2 + box.x0 + box.xlen / 2
                box.entity.y = 0 - cont_y / 2 + box.y0 + box.ylen / 2
                box.entity.z = 0 + cont_z / 2 - box.z0 - box.zlen / 2
            return replace

        b.entity.on_click = tmp(b)

    BoxesController.sorted_boxes = final_solution

    mins = (time.time() - start) / 60
    print("elapsed: " + str(mins))


    editor_camera = EditorCamera(enabled = True)  # add camera controls for orbiting and moving the camera

    app.run()
