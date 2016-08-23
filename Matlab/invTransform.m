function [x,y,z] = invTransform(x,y, H, r, R)
    S = sqrt(H^2+R^2);
    s = sqrt((H^2*r^2)/(R^2) + r^2);
    h = H - (H/R)*r;
    for i = 1:length(x)
        v = sqrt((x(i))^2 + (y(i))^2);
        
        sinP = -x(i)/v;
        cosP = y(i)/v;
        
        angle = atan2(sinP, cosP);
        angle = mod(angle, 2*pi);
        angle = angle*S/R;
        
        y(i) = ((v-s)/(S-s)) * h;
        ri = r + y(i)/h*(R-r);
        x(i) = ri*cos(angle);
        z(i) = ri*sin(angle);
    end
end