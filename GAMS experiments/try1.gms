Sets
    i boxes /b1*b5/
    alias(i,j) ;
   
Scalars
    Wcontainer width of container /3/
    Hcontainer height of container /3/
    Dcontainer depth of container /3/
    M a very large value / 100000 /;

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
    z_(i) front wall of box i
    
    to_maximize variable that represents the quantity to maximize;

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

equations
    max the quantity to maximize
    c_1(i,j) if two boxes are placed then where are they in respect with each other
    
    c_2a(i,j) fix the x coordinate to be packed
    c_2b(i,j) fix the x coordinate to be packed
    c_2c(i,j) fix the z coordinate to be packed
    c_2d(i,j) fix the z coordinate to be packed
    c_2e(i,j) fix the y coordinate to be packed
    c_2f(i,j) fix the y coordinate to be packed
    
    c_3a(i) box does not exceed bin width
    c_3b(i) box does not exceed bin height
    c_3c(i) box does not exceen bin depth
    
    c_4a(i) the width of box is parallel to just one axis
    c_4b(i) only width or depth of box is parallel to axis Z
    c_4c_1(i) height can be parallel only to Z or Y
    c_4c_2(i)
    c_4d_1(i) width height and depth are not parallel to more that one axes
    c_4d_2(i)
    c_4e_1(i)
    c_4e_2(i)
    
    c_7a_1(i,j)
    c_7a_2(i,j)
    c_7b_1(i,j)
    c_7b_2(i,j)
    c_7c_1(i,j)
    c_7c_2(i,j)
    
    c_8a_1(i,j)
    c_8a_2(i,j)
    c_8b_1(i,j)
    c_8b_2(i,j)
    c_8c_1(i,j)
    c_8c_2(i,j)
    
    c_9a_1(i,j)
    c_9a_2(i,j)
    c_9b_1(i,j)
    c_9b_2(i,j)
    c_9c_1(i,j)
    c_9c_2(i,j)
    
    c_10a_1(i,j)
    c_10a_2(i,j)
    c_10b_1(i,j)
    c_10b_2(i,j)
    c_10c_1(i,j)
    c_10c_2(i,j)
    
    c_11_1(i,j)
    c_11_2(i,j)
    c_12_1(i,j)
    c_12_2(i,j)
    c_13_1(i,j)
    c_13_2(i,j)
    c_14_1(i,j)
    c_14_2(i,j)
    c_15(i,j)
    
    c_16(i) definition of x_
    c_17(i) definition on z_
    
    c_22_1(i)
    c_22_2(i)
    c_22_3(i)
;
    
max.. to_maximize =e= sum(i, p(i)*s(i));

c_1(i,j).. r(i,j) + l(i,j) + b(i,j) + f(i,j) + u(i,j) + o(i,j) =e= s(i) + s(j) - 1 $ ((s(i) + s(j) = 2) and not sameas(i,j));

c_2a(i,j).. x(i) + w(i)*X_w(i) + h(i)*(Z_w(i) - Y_h(i) + Z_d(i)) + d(i)*(1 - X_w(i) - Z_w(i) + Y_h(i) - Z_d(i)) =l= x(j) + M *(1 - l(i,j)) $ ((s(i) + s(j) = 2) and not sameas(i,j));
c_2b(i,j).. x(j) + w(j)*X_w(j) + h(j)*(Z_w(j) - Y_h(j) + Z_d(j)) + d(j)*(1 - X_w(j) - Z_w(j) + Y_h(j) - Z_d(j)) =l= x(i) + M *(1 - r(i,j)) $ ((s(i) + s(j) = 2) and not sameas(i,j));
c_2c(i,j).. z(i) + d(i)*Z_d(i) + h(i)*(1 - Z_w(i) - Z_d(i)) + w(i)*Z_w(i) =l= z(j) + M * (1 - b(i,j)) $ ((s(i) + s(j) = 2) and not sameas(i,j));
c_2d(i,j).. z(j) + d(j)*Z_d(j) + h(j)*(1 - Z_w(j) - Z_d(j)) + w(j)*Z_w(j) =l= z(i) + M * (1 - f(i,j)) $ ((s(i) + s(j) = 2) and not sameas(i,j));
c_2e(i,j).. y(i) + h(i)*Y_h(i) + w(i)*(1 - X_w(i) - Z_w(i)) + d(i)*(X_w(i) + Z_w(i) - Y_h(i)) =l= y(j) + M*(1 - u(i,j)) $ ((s(i) + s(j) = 2) and not sameas(i,j));
c_2f(i,j).. y(j) + h(j)*Y_h(j) + w(j)*(1 - X_w(j) - Z_w(j)) + d(j)*(X_w(j) + Z_w(j) - Y_h(j)) =l= y(i) + M*(1 - o(i,j)) $ ((s(i) + s(j) = 2) and not sameas(i,j));

c_3a(i).. x(i) + w(i)*X_w(i) + h(i)*(Z_w(i) - Y_h(i) + Z_d(i)) + d(i)*(1 - X_w(i) - Z_w(i) + Y_h(i) - Z_d(i)) =l= Wcontainer;
c_3b(i).. y(i) + h(i)*Y_h(i) + w(i)*(1 - X_w(i) - Z_w(i)) + d(i)*(X_w(i) + Z_w(i) - Y_h(i)) =l= Hcontainer;
c_3c(i).. z(i) + d(i)*Z_d(i) + h(i)*(1 - Z_w(i) - Z_d(i)) + w(i)*Z_w(i) =l= Dcontainer;

c_4a(i).. X_w(i) + Z_w(i) =l= 1;
c_4b(i).. Z_w(i) + Z_d(i) =l= 1;
c_4c_1(i).. 0 =l= Z_w(i) - Y_h(i) + Z_d(i);
c_4c_2(i).. Z_w(i) - Y_h(i) + Z_d(i) =l= 1;
c_4d_1(i).. 0 =l= 1 - X_w(i) - Z_w(i) + Y_h(i) - Z_d(i);
c_4d_2(i).. 1 - X_w(i) - Z_w(i) + Y_h(i) - Z_d(i) =l= 1;
c_4e_1(i).. 0 =l= X_w(i) + Z_w(i) - Y_h(i);
c_4e_2(i).. X_w(i) + Z_w(i) - Y_h(i) =l= 1;

c_7a_1(i,j).. x(j) - x(i) =l= M*ya(i,j) $ (not sameas(i,j));
c_7a_2(i,j).. x(j) - x(i) =g= M*(ya(i,j) - 1) $ (not sameas(i,j));
c_7b_1(i,j).. x_(i) - x(j) =l= M*xa(i,j) $ (not sameas(i,j));
c_7b_2(i,j).. x_(i) - x(j) =g= M*(xa(i,j) - 1) + 0.5 $ (not sameas(i,j));
c_7c_1(i,j).. (ya(i,j) + xa(i,j) - 1)/2 =l= za(i,j) $ (not sameas(i,j));
c_7c_2(i,j).. za(i,j) =l= (ya(i,j) + xa(i,j))/2 $ (not sameas(i,j));

c_8a_1(i,j).. z(j) - z(i) =l= M*yb(i,j) $ (not sameas(i,j));
c_8a_2(i,j).. z(j) - z(i) =g= M*(yb(i,j) - 1) $ (not sameas(i,j));
c_8b_1(i,j).. z_(i) - z(j) =l= M*xb(i,j) $ (not sameas(i,j));
c_8b_2(i,j).. z_(i) - z(j) =g= M*(xb(i,j) - 1) + 0.5 $ (not sameas(i,j));
c_8c_1(i,j).. (yb(i,j) + xb(i,j) - 1)/2 =l= zb(i,j) $ (not sameas(i,j));
c_8c_2(i,j).. zb(i,j) =l= (yb(i,j) + xb(i,j))/2 $ (not sameas(i,j));

c_9a_1(i,j).. x_(j) - x(i) =l= M*yc(i,j) $ (not sameas(i,j));
c_9a_2(i,j).. x_(j) - x(i) =g= M*(yc(i,j) - 1) + 0.5 $ (not sameas(i,j));
c_9b_1(i,j).. x_(i) - x_(j) =l= M*xc(i,j) $ (not sameas(i,j));
c_9b_2(i,j).. x_(i) - x_(j) =g= M*(xc(i,j) - 1) $ (not sameas(i,j));
c_9c_1(i,j).. (yc(i,j) + xc(i,j) - 1)/2 =l= zc(i,j) $ (not sameas(i,j));
c_9c_2(i,j).. zc(i,j) =l= (yc(i,j) + xc(i,j))/2 $ (not sameas(i,j));

c_10a_1(i,j).. z_(j) - z(i) =l= M*yd(i,j) $ (not sameas(i,j));
c_10a_2(i,j).. z_(j) - z(i) =g= M*(yd(i,j) - 1) + 0.5 $ (not sameas(i,j));
c_10b_1(i,j).. z_(i) - z_(j) =l= M*xd(i,j) $ (not sameas(i,j));
c_10b_2(i,j).. z_(i) - z_(j) =g= M*(xd(i,j) - 1) $ (not sameas(i,j));
c_10c_1(i,j).. (yd(i,j) + xd(i,j) - 1)/2 =l= zd(i,j) $ (not sameas(i,j));
c_10c_2(i,j).. zd(i,j) =l= (yd(i,j) + xd(i,j))/2 $ (not sameas(i,j));

c_11_1(i,j).. (za(i,j) + zb(i,j) - 1)/2 =l= Cs1(i,j) $ (not sameas(i,j));
c_11_2(i,j).. Cs1(i,j) =l= (za(i,j) + zb(i,j))/2 $ (not sameas(i,j));
c_12_1(i,j).. (za(i,j) + zd(i,j) - 1)/2 =l= Cs2(i,j) $ (not sameas(i,j));
c_12_2(i,j).. Cs2(i,j) =l= (za(i,j) + zd(i,j))/2 $ (not sameas(i,j));
c_13_1(i,j).. (zc(i,j) + zb(i,j) - 1)/2 =l= Cs3(i,j) $ (not sameas(i,j));
c_13_2(i,j).. Cs3(i,j) =l= (zc(i,j) + zb(i,j))/2 $ (not sameas(i,j));
c_14_1(i,j).. (zc(i,j) + zd(i,j) - 1)/2 =l= Cs4(i,j) $ (not sameas(i,j));
c_14_2(i,j).. Cs4(i,j) =l= (zc(i,j) + zd(i,j))/2 $ (not sameas(i,j));
c_15(i,j).. Cs1(i,j) + Cs2(i,j) + Cs3(i,j) + Cs4(i,j) =e= u(i,j) + o(i,j) $ (not sameas(i,j));

c_16(i).. x_(i) =e= x(i) + w(i)*X_w(i) + h(i)*(Z_w(i) - Y_h(i) + Z_d(i)) + d(i)*(1 - X_w(i) - Z_w(i) + Y_h(i) - Z_d(i));
c_17(i).. z_(i) =e= z(i) + d(i)*Z_d(i) + h(i)*(1 - Z_w(i) - Z_d(i)) + w(i)*Z_w(i);

c_22_1(i).. x(i) =g= 0;
c_22_2(i).. y(i) =g= 0;
c_22_3(i).. z(i) =g= 0;

model try1 /all/;
solve try1 using mip maximizing to_maximize;
