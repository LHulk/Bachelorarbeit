function [] = runninCalib()
res = [100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950,1000, 1050, 1100, 1150, 1200];
reverse = [6245,6256,6276,6290,6294,6300,6305,6244,6240,6239,6243,6236,6236,6270,6285,6266,6252,6238,6237,6239,6237,6240,6224];
forward = [19202,19147,19126,19127,19154,19114,19102,19092,19056,19013,18993,18952,18989,18949,18964,19002,19034,18949,18956,19043,18939,18951,18954];

scatter(res, reverse, '+', 'MarkerEdgeColor', 'r','LineWidth', 2.5);
hold on;
scatter(res, forward, '+', 'MarkerEdgeColor', 'b','LineWidth', 2.5);

xlabel('Auflösung der Seitenhöhe in Pixel');
ylabel('Laufzeit der Kalibrierung in ms');

axis([0 1330 5000 20000 ])
set(gca,'YTickLabel',num2str(get(gca,'YTick').'))


end

