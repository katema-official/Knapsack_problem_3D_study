from boxmodule_misc import *
from boxmodule_ursina import *

#function that, given a sequence (A, B or C), returns:
#TRUE: if i is before j in seq
#FALSE: if i is after j in seq
#Note that i, j are to considered boxes: So, if we have our array
#of boxes, built at the beginning of the problem, in which the boxes
#have a fixed position, this means that they have a fixed and unique identifier,
#that is the index of the array that contains them (box 1 is the first in
#this fixed array, box 2 is the second in this fixed array...)
def give_X_ij(seq, i, j):
    if i == j: sys.exit(-1) #can't happen
    index_i = 0
    index_j = 0
    for k in range(len(seq)):
        if seq[k] == i:
            index_i = k
            break
    for k in range(len(seq)):
        if seq[k] == j:
            index_j = k
            break
    if index_i<index_j:
        return True
    else:
        return False


def enforce_vertical_stability(x_i, y_i, z_i, xlen, ylen, zlen, P_y, boxes):
    possible_heights = []
    #(1)
    for k in P_y:
        x_j_ = boxes[k].x0
        z_j_ = boxes[k].z0
        if(x_i == x_j_ and z_i == z_j_):
            possible_heights.append(boxes[k].y0 + boxes[k].ylen)
    #(2)
    for k in P_y:
        x_j_ = boxes[k].x0 + boxes[k].xlen
        z_j_ = boxes[k].z0
        if(x_i + xlen == x_j_ and z_i == z_j_):
            possible_heights.append(boxes[k].y0 + boxes[k].ylen)
    #(3)
    for k in P_y:
        x_j_ = boxes[k].x0
        z_j_ = boxes[k].z0 + boxes[k].zlen
        if (x_i == x_j_ and z_i + zlen == z_j_):
            possible_heights.append(boxes[k].y0 + boxes[k].ylen)
    #(4)
    for k in P_y:
        x_j_ = boxes[k].x0 + boxes[k].xlen
        z_j_ = boxes[k].z0 + boxes[k].zlen
        if(x_i + xlen == x_j_ and z_i + zlen == z_j_):
            possible_heights.append(boxes[k].y0 + boxes[k].ylen)
    possible_heights.append(0)
    final_height = max(possible_heights)

    return final_height


#function that places the boxes according to the three sequences
def place_boxes_sequence_triples(a,b,c,boxes):
    first = b[0]
    boxes[first].x0 = 0
    boxes[first].y0 = 0
    boxes[first].z0 = 0
    place_box(0,0,0,boxes[first].xlen,boxes[first].ylen,boxes[first].zlen, boxes[first].color)
    placed_boxes = [first]
    P_x = []
    P_y = []
    P_z = []

    for k in range(1, len(b)):
        current = b[k]  #the currently considered box that we want to place
        for old in placed_boxes:
            #In P_x we put the boxes (already placed) that are to the left than the new one
            if give_X_ij(a,old, current) and give_X_ij(b, old, current) and not give_X_ij(c, old, current):
                P_x.append(old)
            #In P_y we put the boxes (already placed) that are below the new one
            if not give_X_ij(a, old, current) and give_X_ij(b, old, current) and give_X_ij(c, old, current):
                P_y.append(old)
            #In P_z we put the boxes (already placed) that are behind the new one
            if (not give_X_ij(a, old, current) and give_X_ij(b, old, current) and not give_X_ij(c, old, current)) \
                    or (give_X_ij(a, old, current) and give_X_ij(b, old, current) and give_X_ij(c, old, current)):
                        P_z.append(old)

        x_i = 0
        y_i = 0
        z_i = 0

        for kk in range(len(P_x)):
            index_of_box = P_x[kk]
            actual_box = boxes[index_of_box]
            if actual_box.x0 + actual_box.xlen > x_i:
                x_i = actual_box.x0 + actual_box.xlen

        for kk in range(len(P_y)):
            index_of_box = P_y[kk]
            actual_box = boxes[index_of_box]
            if actual_box.y0 + actual_box.ylen > y_i:
                y_i = actual_box.y0 + actual_box.ylen

        for kk in range(len(P_z)):
            index_of_box = P_z[kk]
            actual_box = boxes[index_of_box]
            if actual_box.z0 + actual_box.zlen > z_i:
                z_i = actual_box.z0 + actual_box.zlen

        boxes[current].x0 = x_i
        boxes[current].z0 = z_i

        #now for the vertical stability: let's "push" the boxes below as possible as we can
        y_i = enforce_vertical_stability(x_i, y_i, z_i,
                                         boxes[current].xlen, boxes[current].ylen, boxes[current].zlen, P_y, boxes)

        boxes[current].y0 = y_i

        place_box(x_i, y_i, z_i, boxes[current].xlen, boxes[current].ylen, boxes[current].zlen, boxes[current].color)

        P_x = []
        P_y = []
        P_z = []
        placed_boxes.append(current)
        print(placed_boxes)
        print("x_i = " + str(x_i) + ", y_i = " + str(y_i) + ", z_i = " + str(z_i))


if __name__ == "__main__":
    app = Ursina()

    create_container()

    #generate the boxes
    boxes = []
    #for i in range(10):
    #    boxes.append(randomly_generate_box(50, 50, 50))
    boxes.append(Box("a", 10, 50, 50, rgb(255,0,0)))
    boxes.append(Box("b", 20, 30, 10, rgb(250,163,32)))
    boxes.append(Box("c", 10, 50, 30, rgb(252,244,27)))
    boxes.append(Box("d", 40, 40, 40, rgb(45,247,14)))
    boxes.append(Box("e", 80, 60, 40, rgb(17,32,252)))

    #generate the A sequence randomly. Same for B and C sequence
    a = list(range(5))
    random.shuffle(a)
    print(a)

    b = list(range(5))
    random.shuffle(b)
    print(b)

    c = list(range(5))
    random.shuffle(c)
    print(c)

    #for customization
    if True:
        a = [4, 2, 1, 3, 0]
        b = [2, 4, 0, 3, 1]
        c = [2, 0, 1, 4, 3]

    place_boxes_sequence_triples(a,b,c,boxes)

    print("a")
    editor_camera = EditorCamera(enabled=True)  # add camera controls for orbiting and moving the camera

    app.run()