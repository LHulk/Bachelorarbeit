function [] = running()

res = [100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950,1000, 1050, 1100, 1150, 1200];
time = [1657,1671,1702,1718,1748,1776,1800,1846,1894,1950,1999,2063,2121,2176,2260,2340,2420,2497,2601,2694,2785,2892,2997];

p = polyfit(res, time, 2);
x = linspace(0, 1300, 2000);
y = polyval(p, x);
plot(x,y, 'Color', 'b', 'LineWidth', 3);
hold on;
scatter(res, time, '+', 'MarkerEdgeColor', 'r', 'LineWidth', 2.5);
axis([0 1250 1600 3200]);
xlabel('Auflösung der Seitenhöhe in Pixel');
ylabel('Zeit pro Bild in ms');

end

