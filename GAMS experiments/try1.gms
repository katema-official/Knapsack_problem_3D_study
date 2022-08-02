Sets
    i boxes /b1*b5/
    alias(i,j) ;
   
Scalars
    Wcontainer width of container /3/
    Hcontainer height of container /3/
    Dcontainer depth of container /3/;

Parameters
    w(i) width of box
    /   b1  3
        b2  3
        b3  2
        b4  3
        b5  1/
    
    h(i) height of box
    /   b1  2
        b2  1
        b3  1
        b4  3
        b5  3/
    
    d(i) depth of box
    /   b1  2
        b2  1
        b3  2
        b4  1
        b5  1/
        
    p(i) volume of box;
    

    p(i) = w(i) * h(i) * d(i);
    
Variables
    x(i) x coordinate of box i
    y(i) y coordinate of box i
    z(i) z coordinate of box i
    X_w(i) if the width of box i is parallel to X axis
    Z_w(i) if the width of box i is parallel to Z axis
    Y_h(i) if the height of box i is parallel to Y axis
    Z_d(i) if the depth of box i is parallel to Z axis
    r(i,j) if box i is to the right of box j
    l(i,j) if box i is to the left of box j
    b(i,j) if box i is behind box j
    f(i,j) if box i is in front of box j
    u(i,j) if box i is under box j
    o(i,j) if box i os over box j
    s(i) if box i is packed in the container
    ya(i,j) if xj >= xi
    xa(i,j) if xj < xi_
    yb(i,j) if zj >= zi
    xb(i,j) if zj < zi_
    yc(i,j) if xj_ > xi
    xc(i,j) if xj_ <= xi_
    yd(i,j) if zj_ > zi
    xd(i,j) if zj_ <= zi_
    za(i,j) if xi <= xj < xi_
    zb(i,j) if zi <= zj < zi_
    zc(i,j) if xi < xj_ <= xi_
    zd(i,j) if zi < zj_ <= zi_
    
    Cs1(i,j) if za(ij) and zb(ij)
    Cs2(i,j) if za(ij) and zd(ij)
    Cs3(i,j) if zc(ij) and zb(ij)
    Cs4(i,j) if zc(ij) and zd(ij)
    

    x_(i) right wall of box i
    z_(i) front wall of box i;

binary variables
    X_w
    Z_w
    Y_h
    Z_d
    r
    l
    b
    f
    u
    o
    s
    ya
    xa
    yb
    xb
    yc
    xc
    yd
    xd
    za
    zb
    zc
    zd
    Cs1
    Cs2
    Cs3
    Cs4;
    
positive variables
    x
    y
    z
    x_
    z_









