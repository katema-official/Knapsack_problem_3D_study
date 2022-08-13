import boxmodule_misc as bm
from boxmodule_ursina import Point
from boxmodule_ursina import Box

#Exaustive search of the optimum placement.
def start_ordering(boxes_array, solutions):
    unplaced = boxes_array[:]
    placed = []
    #the available points must be interpreted as the left_bottom_behind points available
    available_points = [Point(0,0,0)]
    order_boxes(unplaced, placed, available_points, solutions)

#recursive function that does an extensive search of the optimal value.
def order_boxes(unplaced, placed, available_points, solutions):

    if(len(solutions) == 0):

        if bm.ratio_occupancy(placed, bm.cont_x, bm.cont_y, bm.cont_z) > 0.5: #len(unplaced) == 0:
            solutions.append(placed)
            for elem in solutions[0]:
                print("{}, {}, {}, {}, {}, {}\n".format(elem.x0, elem.y0, elem.z0, elem.xlen, elem.ylen, elem.zlen))

        else:
            for box_to_place in unplaced:   #for all the boxes that still need to be placed...
                for possible_point in available_points: #for all the points available...
                    for i in range(1,7):    #for all possible permutations of a box...
                        perm = bm.give_permutation(box_to_place.xlen, box_to_place.ylen, box_to_place.zlen, i)
                        if bm.does_box_have_base(box_to_place, possible_point, placed):    #EXTRA control: if the box wouldn't float mid-air...
                            if bm.placeable(placed, perm, possible_point): #if the box doesn't collide with any other box previously placed...
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
