function [x,y,z] = scatterCone(n, m, H, r, R)
    %H is full cone NOT FRUSTUM
    h = H - H/R*r;
    x = zeros(1, n*m);
    y = x;
    z = x;
    for i = 0:n-1
        yi = i/(n-1) * h;
        ri = r + yi/h*(R-r);

        for j = 0:m-1
            angle = j * 2*pi / m;
            xj = ri * cos(angle);
            zj = ri * sin(angle);
            x(i*(m) + j + 1) = xj;
            y(i*(m) + j + 1) = yi;
            z(i*(m) + j + 1) = zj;
        end
    end
   
end