import datetime
import time

from boxmodule_ursina import *
from boxmodule_misc import *
import boxmodule_exaustive_search as bes
#from ursina import curve
#from ursina.prefabs.first_person_controller import FirstPersonController




if __name__ == '__main__':
    app = Ursina()

    x = 1
    y = 1
    z = 1
    create_container()

    custom = False

    boxes = []
    if custom:
        boxes = [Box("a", 20,20,10), Box("a", 10,10,10), Box("a", 20,10,10), Box("a", 10,20,10)]

        """
        box1 = Box("Pippo", 20, 10, 30)
        box2 = Box("Pluto", 40, 30, 20)
        box3 = Box("Pietro", 30, 50, 60)
        box4 = Box("Paperino", 50, 30, 80)
        box5 = Box("Topolino", 30, 40, 10)
        box6 = Box("Minnie", 50, 20, 70)
        box7 = Box("Aaa", 70, 40, 60)
        box8 = Box("Bbb", 30, 10, 20)
        boxes = [box1, box2, box3, box4, box5, box6, box7, box8]
        """
    else:
        boxes = []
        for i in range(50):
            boxes.append(randomly_generate_box(50,50,50))

    start = time.time()

    solutions = []
    bes.start_ordering(boxes, solutions)
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
