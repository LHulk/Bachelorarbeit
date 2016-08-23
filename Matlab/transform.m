function [x,y] = transform(x,y,z, H, r, R)
    S = sqrt(H^2+R^2);
    s = sqrt((H^2*r^2)/(R^2) + r^2);
    h = H - H/R*r;
    for i = 1:length(x)
        currS = s + y(i)/h * (S-s);

        cosP = x(i)/(r + (y(i)/h) * (R-r));
        sinP = z(i)/(r + (y(i)/h) * (R-r));

        angle = atan2(sinP, cosP);
        angle = mod(angle, 2*pi);
        angle = angle*(R/S);

        x(i) = -currS * sin(angle);
        y(i) = currS * cos(angle);
    end
end