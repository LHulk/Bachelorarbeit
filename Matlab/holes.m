function [] = holes()

d = [200:50:1000];
e = [1623,2721,7799,19337,40168,72349,116676,173543,245216,329354,423877,527094,638029,756185,881294,1013880,1153448];

p1 = polyfit(d, e, 2);
x = linspace(100, 1300, 1200);
y1 = polyval(p1, x);

h1 = plot(x,y1, 'Color', 'b', 'LineWidth', 3);
hold on;
scatter(d, e, '+', 'MarkerEdgeColor', 'r', 'LineWidth', 2.5);

axis([180 1050 -10000 1200000]);
xlabel('Auflösung der Seitenhöhe in Pixel');
ylabel('Anzahl der Defekte');
%legend([h1, h2], 'bikubisch', 'linear');

set(gca,'YTickLabel',num2str(get(gca,'YTick').'))

end
