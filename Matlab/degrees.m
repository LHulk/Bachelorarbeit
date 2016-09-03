function [] = degrees()
values = load('reprojByDeg.txt', '-ascii');

s = size(values)

for i = 1:(s(1)/2)
    sx = values(i,:)
    sy = values(i+1,:);
    norms(:,i) = sqrt(sx.^2 + sy.^2);
    avgs(i) = mean(norms(:,i));
end

boxplot(norms, 'Labels',{'0', '1','2','3','4','5','6','7','8','9','10','11','12'});


% 
% scatter(x00,y00, 'x');
% hold on;
% scatter(x25,y25, 'x');
% hold on;
% scatter(x50,y50, 'x');
% hold on;
% scatter(x75,y75, 'x');
% hold on;
% scatter(x100,y100, 'x');
% hold on;
% scatter(x125,y125, 'x');


end

