#the container (bin) dimensions
cont_x = 80#120
cont_y = 80#60
cont_z = 80#80

#function that tells if, given two boxes with their tentative LBB point and their
#current orientation (width, height and depth), they overlap (one is inside the other).
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

#function that tells if a box with LBB point "point" and dimensions "tuple"
#can be placed in the bin. The arguments are interpreted as:
#boxes placed, tuple (x,y,z), point (x0,y0,z0)
#The last two arguuments regard the box we'd like to place in the given LBB point.
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





#function that returns the area, enclosed between left_z, right_z, left_x and right_x
#that could sustain base_box, which we would like to place above
def base_area(left_z, right_z, left_x, right_x, base_box):
    true_left_z = max(left_z, base_box.z0)
    true_right_z = min(right_z, base_box.z0 + base_box.zlen)
    true_left_x = max(left_x, base_box.x0)
    true_right_x = min(right_x, base_box.x0 + base_box.xlen)
    if true_right_z > true_left_z and true_right_x > true_left_x:
        return (true_right_z - true_left_z) * (true_right_x - true_left_x)
    return 0

#function used to check if "box" has a base to be placed, in order for it to not fall.
#point is its LBB (tentative)
def does_box_have_base(box, point, placed_boxes):
    if point.y == 0:
        return True

    #first: find the boxes that could constitute the base of the new box
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

    #third: check if the box has a base large enough to be sustained.
    #The choice that the base is large enough when it is at least half
    #of the base of the box is arbitrary.
    if final_base_area >= (box.zlen * box.xlen) / 2:
        return True

    return False




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

#Function that tells the total amount of covered volume, given the current configuration
#of the boxes, in the container
def ratio_occupancy(placed_boxes, container_x, container_y, container_z):
    volume_occupied = 0
    for b in placed_boxes:
        volume_occupied += b.xlen*b.ylen*b.zlen
    total_volume = container_x*container_y*container_z
    return volume_occupied / total_volume










