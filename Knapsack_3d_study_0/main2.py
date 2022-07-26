import ursina

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


#function that checks if a given newly placed box falls above another previously
#placed box. If that is so, it returns the height of its upper surface.
#(function written mainly to compress the code used in the enforce_vertical_stability function)
#NOTE: both x_0 and x0_plus_len (same goes for z) must be checked!
def check_height(x0, x0_plus_len, z0, z0_plus_len, x_i, z_i, box_already_placed):
    for x_j_ in range(x0, x0_plus_len + 1):
        for z_j_ in range(z0, z0_plus_len + 1):
            if(x_i == x_j_ and z_i == z_j_):
                return box_already_placed.y0 + box_already_placed.ylen
    return 0


def enforce_vertical_stability(x_i, y_i, z_i, xlen, ylen, zlen, P_y, boxes):
    possible_heights = []
    #(1)
    for k in P_y:
        h = check_height(boxes[k].x0, boxes[k].x0 + boxes[k].xlen - 1,
                                boxes[k].z0, boxes[k].z0 + boxes[k].zlen - 1,
                                x_i, z_i, boxes[k])
        if h != 0: possible_heights.append(h)
    #(2)
    for k in P_y:
        h = check_height(boxes[k].x0 + 1, boxes[k].x0 + boxes[k].xlen,
                         boxes[k].z0, boxes[k].z0 + boxes[k].zlen - 1,
                         x_i + xlen, z_i, boxes[k])
        if h != 0: possible_heights.append(h)
    #(3)
    for k in P_y:
        h = check_height(boxes[k].x0, boxes[k].x0 + boxes[k].xlen - 1,
                         boxes[k].z0 + 1, boxes[k].z0 + boxes[k].zlen,
                         x_i, z_i + zlen, boxes[k])
        if h != 0: possible_heights.append(h)
    #(4)
    for k in P_y:
        h = check_height(boxes[k].x0 + 1, boxes[k].x0 + boxes[k].xlen,
                         boxes[k].z0 + 1, boxes[k].z0 + boxes[k].zlen,
                         x_i + xlen, z_i + zlen, boxes[k])
        if h != 0: possible_heights.append(h)


    possible_heights.append(0)
    final_height = max(possible_heights)

    return final_height



#my take on enforcing vertical stability
def enforce_vertical_stability_2(x_i, y_i, z_i, xlen, ylen, zlen, P_y, boxes):
    max_h = 0
    for k in P_y:
        k_x0 = boxes[k].x0
        k_z0 = boxes[k].z0
        k_x1 = boxes[k].x0 + boxes[k].xlen
        k_z1 = boxes[k].z0 + boxes[k].zlen
        #the box k is below the box we have to place if one of its four points
        #falls under the lower surface of the box to be placed
        if (k_x0 >= x_i and k_x0 < x_i + xlen and k_z0 >= z_i and k_z0 < z_i + zlen) or \
            (k_x1 > x_i and k_x1 <= x_i + xlen and k_z0 >= z_i and k_z0 < z_i + zlen) or \
            (k_x0 >= x_i and k_x0 < x_i + xlen and k_z1 > z_i and k_z1 <= z_i + zlen) or \
            (k_x1 > x_i and k_x1 <= x_i + xlen and k_z1 > z_i and k_z1 <= z_i + zlen):
                if boxes[k].y0 + boxes[k].ylen > max_h:
                    max_h = boxes[k].y0 + boxes[k].ylen
        else:
            #at the same time, the box k is below the box we have to place if one of the
            #four points of the box we have to place falls above the box k
            btp_x0 = x_i    #btp = box to place
            btp_z0 = z_i
            btp_x1 = x_i + xlen
            btp_z1 = z_i + zlen
            if (btp_x0 >= k_x0 and btp_x0 < k_x1 and btp_z0 >= k_z0 and btp_z0 < k_z1) or \
                    (btp_x1 > k_x0 and btp_x1 <= k_x1 and btp_z0 >= k_z0 and btp_z0 < k_z1) or \
                    (btp_x0 >= k_x0 and btp_x0 < k_x1 and btp_z1 > k_z0 and btp_z1 <= k_z1) or \
                    (btp_x1 > k_x0 and btp_x1 <= k_x1 and btp_z1 > k_z0 and btp_z1 <= k_z1):
                        if boxes[k].y0 + boxes[k].ylen > max_h:
                            max_h = boxes[k].y0 + boxes[k].ylen

    return max_h


#function that places the boxes according to the three sequences
def place_boxes_sequence_triples(a,b,c,boxes):
    first = b[0]
    boxes[first].x0 = 0
    boxes[first].y0 = 0
    boxes[first].z0 = 0
    #place_box(0,0,0,boxes[first].xlen,boxes[first].ylen,boxes[first].zlen, boxes[first].color)
    placed_boxes = [first]
    P_x = []
    P_y = []
    P_z = []

    for k in range(1, len(b)):
        current = b[k]  #the currently considered box that we want to place
        for old in placed_boxes:
            #In P_x we put the boxes (already placed) that are to the left than the new one
            if give_X_ij(a, old, current) and give_X_ij(b, old, current) and not give_X_ij(c, old, current):
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
        y_i = enforce_vertical_stability_2(x_i, y_i, z_i,
                                         boxes[current].xlen, boxes[current].ylen, boxes[current].zlen, P_y, boxes)

        boxes[current].y0 = y_i

        P_x = []
        P_y = []
        P_z = []
        placed_boxes.append(current)

def place_boxes_in_space(boxes):
    for box in boxes:
        box.entity = place_box(box.x0, box.y0, box.z0, box.xlen, box.ylen, box.zlen, box.color)

def destroy_placed_boxes_in_space(boxes):
    for box in boxes:
        ursina.destroy(box.entity)

#def replace_boxes_in_space(boxes):
#    for box in boxes:
#        replace_box(box)


#define the volume of the container occupied by the boxes. Note that
#to count the volume occupied we only take into account boxes that are
#completely inside the container.
def volume_occupied(boxes, container_x, container_y, container_z):
    #volume_total = container_x*container_y*container_z
    volume_summed = 0
    for box in boxes:
        if box.x0 + box.xlen <= container_x and \
            box.y0 + box.ylen <= container_y and \
            box.z0 + box.zlen <= container_z:
                volume_summed += box.xlen*box.ylen*box.zlen
    return volume_summed

def generate_neighbours(a_best, b_best, c_best, boxes):
    n = len(boxes)
    # The neighbourhood of each solution is defined as one of these five permutations:
    # either exchange two boxes from one of the sequences; exchange two boxes in
    # sequences A and B; exchange two boxes in sequences A and C; exchange two boxes
    # in sequences C and B; or exchange two boxes in all sequences.
    neighbours = []

    i = random.choice(list(range(n)))
    j = random.choice([e for e in list(range(n)) if e != i])
    x0 = a_best.index(i)    #index of box i in a
    y0 = a_best.index(j)    #index of box j in a
    x1 = b_best.index(i)
    y1 = b_best.index(j)
    x2 = c_best.index(i)
    y2 = c_best.index(j)

    #first neighbour
    n1 = (a_best[:], b_best[:], c_best[:])
    list_chosen = random.choice([0,1,2]) #extremes included
    x = n1[list_chosen].index(i)
    y = n1[list_chosen].index(j)
    n1[list_chosen][x], n1[list_chosen][y]= n1[list_chosen][y], n1[list_chosen][x]
    neighbours.append(n1)

    #second neighbour
    n2 = (a_best[:], b_best[:], c_best[:])
    n2[0][x0], n2[0][y0] = n2[0][y0], n2[0][x0]
    n2[1][x1], n2[1][y1] = n2[1][y1], n2[1][x1]
    neighbours.append(n2)

    #third neighbour
    n3 = (a_best[:], b_best[:], c_best[:])
    n3[0][x0], n3[0][y0] = n3[0][y0], n3[0][x0]
    n3[2][x2], n3[2][y2] = n3[2][y2], n3[2][x2]
    neighbours.append(n3)

    #fourth neighbour
    n4 = (a_best[:], b_best[:], c_best[:])
    n4[1][x1], n4[1][y1] = n4[1][y1], n4[1][x1]
    n4[2][x2], n4[2][y2] = n4[2][y2], n4[2][x2]
    neighbours.append(n4)

    #fifth neighbour
    n5 = (a_best[:], b_best[:], c_best[:])
    n5[0][x0], n5[0][y0] = n5[0][y0], n5[0][x0]
    n5[1][x1], n5[1][y1] = n5[1][y1], n5[1][x1]
    n5[2][x2], n5[2][y2] = n5[2][y2], n5[2][x2]
    neighbours.append(n5)

    return neighbours


def generate_neighbours_extended_rotation_1(a_best, b_best, c_best, boxes, p):
    #this function is an extension of the generate_neighbour. In addition to the
    #neighbours already generated, we allow ONE of the boxes to change its orientation
    #with a certain probability.
    neighbours = generate_neighbours(a_best, b_best, c_best, boxes)
    u = random.uniform(0,1)
    if u < p:
        #with a certain probability, we choose a box...
        b = random.choice(boxes)
        #...and randomly change its orientation
        b.xlen, b.ylen, b.zlen = give_permutation(b.xlen, b.ylen, b.zlen, random.choice(list(range(1,7))))
    return neighbours

#other approaches would be to incorporate the rotation changes directly into the
#neighbour solutions, so that they can be evaluated properly when considering
#to accept that solution or not.

#to do so, we can hard-code this directly into the neighbours, but we can also choose
#a neighbour solution as we did before, and then, with a certain probability, rotate
#each of the box. We can also fix a priori a number of boxes to rotate, like 1 (would
#be the approach done before), or randomize it.

#let's try to implement the one that, with a certain probability, rotates
#each of the boxes.
def generate_neighbours_extended_rotation_2(a_best, b_best, c_best, boxes, p):
    #this function is an extension of the generate_neighbour. In addition to the
    #neighbours already generated, we allow ONE of the boxes to change its orientation
    #with a certain probability.
    neighbours = generate_neighbours(a_best, b_best, c_best, boxes)
    for b in boxes:
        u = random.uniform(0,1)
        if u < p:
            #with a certain probability, change the orientation of a box
            b.xlen, b.ylen, b.zlen = give_permutation(b.xlen, b.ylen, b.zlen, random.choice(list(range(1,7))))
    return neighbours





class SimulatedAnnealing:

    a = []
    b = []
    c = []
    boxes = []

    temperature = 0.2
    beta = 0.2
    alpha = 0.002

    a_best = []
    b_best = []
    c_best = []

    best_volume = 0

    max_number_of_iterations = 10000
    current_iteration = 0



    def initialize(cls, a, b, c, boxes):
        cls.boxes = boxes
        place_boxes_sequence_triples(a, b, c, cls.boxes)
        cls.a_best = a
        cls.a = a
        cls.b_best = b
        cls.b = b
        cls.c_best = c
        cls.c = c
        cls.best_volume = volume_occupied(cls.boxes, cont_x, cont_y, cont_z)
        place_boxes_in_space(cls.boxes)



    def make_a_step(cls):
            step_debug = False
        #here the Simulated Annealing algorithm will modify the current solution
        #and find another one
        #while(cls.current_iteration < cls.max_number_of_iterations):
            neighbours = generate_neighbours_extended_rotation_2(cls.a_best, cls.b_best, cls.c_best, cls.boxes, 0.2)
            neighbour_chosen = random.choice(neighbours)
            boxes_neighbour_chosen = cls.boxes[:]
            place_boxes_sequence_triples(neighbour_chosen[0], neighbour_chosen[1],
                neighbour_chosen[2], boxes_neighbour_chosen)
            neighbour_volume = volume_occupied(boxes_neighbour_chosen, cont_x, cont_y, cont_z)
            found_better = False
            if step_debug: print("neighbour_volume = " + str(neighbour_volume/(cont_x*cont_y*cont_z)) +
                  ", best_volume = " + str(cls.best_volume/(cont_x*cont_y*cont_z)))
            if neighbour_volume >= cls.best_volume:
                found_better = True
            else:
                #We accept a worse solution at random, but the chance of
                #doing so decreases with the temperature
                cls.temperature = cls.temperature / (1 + cls.beta * cls.temperature)
                delta = (cls.best_volume - neighbour_volume) / cls.best_volume
                i = random.uniform(0,1)
                if i < math.e ** (-delta / cls.temperature):
                    found_better = True
                    if step_debug: print("worst, but probability was " + str(math.e ** (-delta / cls.temperature)) + \
                          " and temperature is " + str(cls.temperature) + " and delta was " + str(delta))
                else:
                    #increase temperature
                    cls.temperature = cls.temperature / (1 - cls.alpha * cls.temperature)
                    if step_debug: print("temperature increased to " + str(cls.temperature))

            if found_better:
                #cls.current_iteration = cls.current_iteration + 1
                cls.a_best = neighbour_chosen[0]
                cls.b_best = neighbour_chosen[1]
                cls.c_best = neighbour_chosen[2]
                if step_debug:
                    print(cls.a_best)
                    print(cls.b_best)
                    print(cls.c_best)
                cls.best_volume = neighbour_volume
                #destroy_placed_boxes_in_space(cls.boxes)
                cls.boxes = boxes_neighbour_chosen
                #place_boxes_in_space(cls.boxes)
            #cls.current_iteration += 1
            #if cls.current_iteration > cls.max_number_of_iterations - 1:
                #print("best_volume = " + str(cls.best_volume/(cont_x*cont_y*cont_z)))
                #cls.current_iteration = 0


#see class above
def input(key):
    if key == "l":
        SimulatedAnnealing.make_a_step(SimulatedAnnealing)
        destroy_placed_boxes_in_space(SimulatedAnnealing.boxes)
        place_boxes_in_space(SimulatedAnnealing.boxes)

    #for bigger problems, let's use an authomatic approach
    if key == "k":
        for i in range(SimulatedAnnealing.max_number_of_iterations):
            SimulatedAnnealing.make_a_step(SimulatedAnnealing)
        destroy_placed_boxes_in_space(SimulatedAnnealing.boxes)
        place_boxes_in_space(SimulatedAnnealing.boxes)

    if key == "j":
        now = time.time()
        duration = 10*60
        while(time.time() - now <= duration):
            for i in range(100):
                SimulatedAnnealing.make_a_step(SimulatedAnnealing)
        destroy_placed_boxes_in_space(SimulatedAnnealing.boxes)
        place_boxes_in_space(SimulatedAnnealing.boxes)
        print("best_volume = " + str(SimulatedAnnealing.best_volume / (cont_x * cont_y * cont_z)))


if __name__ == "__main__":
    app = Ursina()

    create_container()

    #generate the boxes
    boxes = []
    #for i in range(10):
    #    boxes.append(randomly_generate_box(50, 50, 50))

    read_from_file =True

    if not read_from_file:
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
    else:
        file = open("C:\\Users\\aless\\Desktop\\figs_measures.txt", "r")
        file = open("C:\\Users\\aless\\Desktop\\Knapsack_problem_3D_study\\template.txt", "r")
        number_of_boxes = 0
        for line in file:
            box_description = line.split("|")
            if len(box_description) == 4:
                n = len(box_description[3])
                print(box_description[3][:n-1])
                boxes.append(Box(box_description[3][:n-1], int(box_description[0]), int(box_description[1]), int(box_description[2])))
                number_of_boxes += 1
            if len(box_description) == 5:
                n = int(box_description[3])
                for i in range(int(box_description[4])):
                    boxes.append(Box(box_description[3], int(box_description[0]), int(box_description[1]), int(box_description[2]), rgb(n*30,((n*7)**2 * 7) % 255,255 - n*30)))
                    number_of_boxes += 1

        a = list(range(number_of_boxes))
        random.shuffle(a)
        print(a)

        order_by_volume = True
        if order_by_volume:
            boxes.sort(key=lambda x : -x.xlen*x.ylen*x.zlen)
            b = list(range(number_of_boxes))
            print(b)
        else:
            b = list(range(number_of_boxes))
            random.shuffle(b)
            print(b)

        c = list(range(number_of_boxes))
        random.shuffle(c)
        print(c)



    #for customization
    if False:
        a = [4, 2, 1, 3, 0]
        b = [2, 4, 0, 3, 1]
        c = [2, 0, 1, 4, 3]
        a = [1, 3, 4, 2, 0]
        b = [4, 0, 1, 2, 3]
        c = [4, 2, 0, 3, 1]

        a = [1, 0, 3, 2, 4]
        b = [3, 4, 2, 0, 1]
        c = [2, 1, 3, 4, 0]

        a = [2, 4, 3, 1, 0]
        b = [2, 1, 0, 4, 3]
        c = [3, 2, 0, 1, 4]

        a = [1, 3, 4, 2, 0]
        b = [4, 2, 1, 3, 0]
        c = [4, 2, 0, 3, 1]

        a = [1, 0, 3, 2, 4]
        b = [3, 4, 1, 2, 0]
        c = [3, 4, 2, 0, 1]

        a = [2, 1, 0, 3, 4]
        b = [2, 3, 4, 0, 1]
        c = [0, 3, 2, 4, 1]

        #one of the best solutions (277000 volume, everything except red box)
        #[4, 3, 0, 1, 2]
        #[1, 3, 2, 4, 0]
        #[4, 2, 1, 0, 3]
        #another
        #[4, 0, 2, 1, 3]
        #[1, 2, 3, 4, 0]
        #[4, 3, 1, 2, 0]
        #even better, might think of adding to the volume function (fitness, basically)
        #a penality term that penalizes solutions where a big box is sustained by a small one.
        #It could at least be chosen if the two solutions have the same fitness.
        #[4, 0, 1, 2, 3]
        #[1, 2, 3, 4, 0]
        #[4, 3, 2, 1, 0]


    SimulatedAnnealing.initialize(SimulatedAnnealing, a, b, c, boxes)



    print(volume_occupied(boxes, cont_x, cont_y, cont_z))

    editor_camera = EditorCamera(enabled=True)  # add camera controls for orbiting and moving the camera

    app.run()