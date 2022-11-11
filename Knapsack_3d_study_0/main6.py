#used to study how the C port of main3 behaves
import math
import os

import ursina
from boxmodule_ursina import *
from boxmodule_misc import *

file_dir = os.path.dirname(os.path.realpath('__file__'))
file_name = os.path.join(file_dir, '..\\Branch_and_Bound_4\\results\\progresses4.txt')

file = open(file_name, "r")
#file = open("C:\\Users\\aless\\Desktop\\Knapsack_problem_3D_study\\C_stuff\\hmg_based_implementation_v1\\" +
#k            "Interesting_data\\final_results_relaxed.txt", "r")

def isOverlapping1D(box1,box2):
    return box1[1] > box2[0] and box2[1] > box1[0]

def check_if_boxes_overlap_2(boxes):
    overlap = False
    for i in range(len(boxes)):
        boxes_copy = boxes[i + 1:]
        for j in range(len(boxes_copy)):
            x0_a = boxes[i].x0
            x0_b = boxes_copy[j].x0
            xlen_a = x0_a + boxes[i].xlen
            xlen_b = x0_b + boxes_copy[j].xlen

            box1x = (x0_a, xlen_a)
            box2x = (x0_b, xlen_b)

            y0_a = boxes[i].y0
            y0_b = boxes_copy[j].y0
            ylen_a = y0_a + boxes[i].ylen
            ylen_b = y0_b + boxes_copy[j].ylen

            box1y = (y0_a, ylen_a)
            box2y = (y0_b, ylen_b)

            z0_a = boxes[i].z0
            z0_b = boxes_copy[j].z0
            zlen_a = z0_a + boxes[i].zlen
            zlen_b = z0_b + boxes_copy[j].zlen

            box1z = (z0_a, zlen_a)
            box2z = (z0_b, zlen_b)

            if isOverlapping1D(box1x, box2x) and isOverlapping1D(box1y, box2y) and isOverlapping1D(box1z, box2z):
                print(str(i) + " e " + str(i + j) + " si sovrappongono")
                overlap = True
    return overlap



class MyBoxes:
    boxes_new = []

def make_a_step():
    destroy_placed_boxes_in_space(MyBoxes.boxes_new)
    MyBoxes.boxes_new.clear()

    n_boxes = int(file.readline()[:-1])
    print(n_boxes)

    volume_occupied_absolute = 0

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
                                     rgb(xlen + ((xlen*ylen*zlen) + 27) * 15 % 255,
                                         ylen + ((xlen*ylen*zlen) + 54) * 25 % 255,
                                         zlen + ((xlen*ylen*zlen) + 81) * 20 % 255), x0, y0, z0, 1))
        volume_occupied_absolute += xlen*ylen*zlen

    line = file.readline()[:-1]
    if line != "---":
        n_boxes_volumes = int(line)
        print(str(n_boxes_volumes))
        for i in range(n_boxes_volumes):
            line = file.readline()[:-1]
            elements = line.split(" ")
            print(elements)
            xlen = int(elements[0])
            ylen = int(elements[1])
            zlen = int(elements[2])
            x0 = int(elements[3])
            y0 = int(elements[4])
            z0 = int(elements[5])
            MyBoxes.boxes_new.append(Box("a", xlen, ylen, zlen, rgb(255, 0, 0), x0, y0, z0, 0.5))
        line = file.readline()[:-1]
    print(line)
    line = file.readline()[:-1]
    print(line)

    print(volume_occupied_absolute / (cont_x*cont_y*cont_z))

    place_boxes_in_space(MyBoxes.boxes_new, True)
    # print("best_volume = " + str(SimulatedAnnealing.best_volume / (cont_x * cont_y * cont_z)))




def input(key):
    if key == "l":
        make_a_step()
    if key == "k":
        for i in range(100):
            make_a_step()
    if key == "j":
        for i in range(1000):
            make_a_step()
    if key == "h":
        for i in range(10000):
            make_a_step()


def place_boxes_in_space(boxes, all=False):
    boxes_effectively_placed = []
    volume_covered = 0
    for box in boxes:
        if all == True:
            box.entity = place_box(box.x0, box.y0, box.z0, box.xlen, box.ylen, box.zlen, box.color, box.alpha)
            boxes_effectively_placed.append(box)
            volume_covered += box.xlen * box.ylen * box.zlen
        else:
            if(box.x0 + box.xlen <= cont_x and box.y0 + box.ylen <= cont_y and box.z0 + box.zlen <= cont_z):
                print("aaaaa")
                box.entity = place_box(box.x0, box.y0, box.z0, box.xlen, box.ylen, box.zlen, box.color, box.alpha)
                boxes_effectively_placed.append(box)
                volume_covered += box.xlen * box.ylen * box.zlen
    #print(volume_covered/(cont_x*cont_y*cont_z))

    if check_if_boxes_overlap_2(boxes_effectively_placed): print("Si sovrappongono")

def destroy_placed_boxes_in_space(boxes):
    for box in boxes:
        ursina.destroy(box.entity)

if __name__ == "__main__":
    app = Ursina(borderless = False)
    create_container()
    finished = False


    editor_camera = EditorCamera(enabled=True)  # add camera controls for orbiting and moving the camera

    app.run()





