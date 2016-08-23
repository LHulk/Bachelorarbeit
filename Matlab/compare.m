function [] = compare()

[x,y,z] = scatterCone(10,12, 110, 13, 60);
[a,b] = transform(x,y,z, 110, 13, 60);
scatter(a,b);
axis([-150 150 -150 150]);
figure;

[d,e,f] = invTransform(a,b, 110, 13, 60);
scatter3(d,e,f);
hold on;
scatter3(x,y,z);

end

