function [] = running()

res = [100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950,1000, 1050, 1100, 1150, 1200];
time1 = [1657,1671,1702,1718,1748,1776,1800,1846,1894,1950,1999,2063,2121,2176,2260,2340,2420,2497,2601,2694,2785,2892,2997];
time2 = [1220,1237,1243,1250,1268,1278,1295,1309,1335,1381,1396,1438,1455,1501,1541,1584,1612,1657,1702,1756,1802,1862, 1917];

p1 = polyfit(res, time1, 2);
p2 = polyfit(res, time2, 2);
x = linspace(0, 1300, 2000);
y1 = polyval(p1, x);
y2 = polyval(p2, x);

h1 = plot(x,y1, 'Color', 'b', 'LineWidth', 3);
hold on;
scatter(res, time1, '+', 'MarkerEdgeColor', 'r', 'LineWidth', 2.5);
hold on;
h2 = plot(x,y2, 'Color', [0 0.6 0], 'LineWidth', 3);
hold on;
scatter(res, time2, '+', 'MarkerEdgeColor', 'r', 'LineWidth', 2.5);
axis([0 1250 1100 3200]);
xlabel('Auflösung der Seitenhöhe in Pixel');
ylabel('Zeit pro Bild in ms');
legend([h1, h2], 'bikubisch', 'linear');

end
