function [] = runninCalib()
res = 100:50:1200;
%reverse = [6245,6256,6276,6290,6294,6300,6305,6244,6240,6239,6243,6236,6236,6270,6285,6266,6252,6238,6237,6239,6237,6240,6224];
time1 = [265,582,1025,1596,2283,3095,4039,5092,6270,7626,9007,10566,12242,14039,15957,18011,20179,22458,24895,27435,30092,32917, 35821];
%forward = [19202,19147,19126,19127,19154,19114,19102,19092,19056,19013,18993,18952,18989,18949,18964,19002,19034,18949,18956,19043,18939,18951,18954];
time2 = [17968,17969,18003,18008,17978,17974,18009,18257,18129,18188,18277,17987,17976,17970,18177,17981,18126,18043,18032,18035,18116,18099,17990];


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
axis([0 1250 0 50000]);
xlabel('Auflösung der Seitenhöhe in Pixel');
ylabel('Laufzeit der Kalibrierung in ms');
legend([h1, h2], 'Rückwärtsentfaltung', 'Vorwärtsentfaltung');

%scatter(res, reverse, '+', 'MarkerEdgeColor', 'r','LineWidth', 2.5);
%hold on;
%scatter(res, forward, '+', 'MarkerEdgeColor', 'b','LineWidth', 2.5);



%axis([0 1330 5000 20000 ])
set(gca,'YTickLabel',num2str(get(gca,'YTick').'))


end

