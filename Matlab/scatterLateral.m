function [x,y] = scatterLateral(n, m, H, r, R)
    %n circles, m
    S = sqrt(H^2+R^2);
    s = sqrt((H^2*r^2)/(R^2) + r^2);
    maxAngle = 2*pi*R/S;
    angleOffset = pi/2;
    
    h = H - H/R*r;
    x = zeros(1, n*m);
    y = x;
    for i = 0:n-1
        currS = s + i/(n-1)*(S-s);
        for j = 0:m-1
            currAngle = j/(m)*maxAngle + angleOffset;
            x(i*(m) + j + 1) = currS * cos(currAngle);
            y(i*(m) + j + 1) = currS * sin(currAngle);
        end
    end
end

