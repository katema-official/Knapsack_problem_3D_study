#used to study how the C port of main3 behaves
import math

import ursina
from boxmodule_ursina import *

file = open("C:\\Users\\aless\\Desktop\\Knapsack_problem_3D_study\\C_stuff\\final_results.txt", "r")

class MyBoxes:
    boxes_new = []

def make_a_step():
    destroy_placed_boxes_in_space(MyBoxes.boxes_new)
    MyBoxes.boxes_new.clear()

    n_boxes = int(file.readline()[:-1])
    print(n_boxes)

    for i in range(n_boxes):
        line = file.readline()[:-1]
        elements = line.split(" ")
        print(elements)
        xlen = int(elements[0])
        ylen = int(elements[1])
        zlen = int(elements[2])
        x0 = int(elements[3])
        y0 = int(elements[4])
        z0 = int(elements[5])
        MyBoxes.boxes_new.append(Box("a", xlen, ylen, zlen,
                                     rgb(math.e ** (i + 5) * 10 % 255, math.e ** (i + 10) * 10 % 255,
                                         math.e ** (i + 15) * 10 % 255), x0, y0, z0))
    a = [int(elem) for elem in file.readline()[:-1].split(" ")[:-1]]
    b = [int(elem) for elem in file.readline()[:-1].split(" ")[:-1]]
    c = [int(elem) for elem in file.readline()[:-1].split(" ")[:-1]]

    place_boxes_in_space(MyBoxes.boxes_new)
    # print("best_volume = " + str(SimulatedAnnealing.best_volume / (cont_x * cont_y * cont_z)))

def input(key):
    if key == "l":
        make_a_step()
    if key == "k":
        for i in range(10000):
            make_a_step()


def place_boxes_in_space(boxes):
    for box in boxes:
        box.entity = place_box(box.x0, box.y0, box.z0, box.xlen, box.ylen, box.zlen, box.color)

def destroy_placed_boxes_in_space(boxes):
    for box in boxes:
        ursina.destroy(box.entity)

if __name__ == "__main__":
    app = Ursina()
    create_container()
    finished = False


    editor_camera = EditorCamera(enabled=True)  # add camera controls for orbiting and moving the camera

    app.run()





