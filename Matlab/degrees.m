function [] = degrees()
values = load('reprojByDeg.txt', '-ascii');

s = size(values);
num = s(1)/2;

for i = 1:num
    sx = values(i,:);
    sy = values(i+1,:);
    norms(:,i) = sqrt(sx.^2 + sy.^2);
    avgs(i) = mean(norms(:,i));
end

x = 0:1:num-1
h1 = bar(x,avgs, 0.5);
x = -10:1:22;
y = mean(avgs)*ones(1,22+10+1);
hold on;
h2 = plot(x,y,'m--', 'LineWidth', 2);
axis([-1 num 0 3])
legend([h2],{strcat('insg. Durschnitt:', num2str(mean(avgs)))});

id = 0:1:16;
set(gca, 'XTick',id);
%line([12.5, 12.5],[0,4], 'LineWidth', 3, 'Color', 'r');






%boxplot(norms, 'Labels',{'0', '1','2','3','4','5','6','7','8','9','10','11','12'});


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

