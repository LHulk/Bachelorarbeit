function [] = reprojReverse()

calibx1 = [0.612427,-0.409851,-0.461853,0.237305,0.115601,0.471436,0.545837,-0.672729,-0.349609,-0.035248,-0.000854,0.009827,0.596375,-0.617981,0.112335,-0.220215,-0.186279,-0.275208,0.496948,-0.555115,0.940796,-0.236847,0.007080,-0.751953,0.450500,-0.941101,1.549469,-0.613800,0.507507,-0.188232];
caliby1 = [-0.366516,-0.383728,0.141388,0.561310,0.464264,-0.489075,-0.245514,-0.093231,0.014008,0.116425,0.265747,-0.258331,-0.080048,0.159180,-0.079437,0.051147,0.210861,0.128784,-0.212006,0.306824,0.006195,-0.277588,0.053345,0.520355,-0.569153,0.535828,0.214417,-0.871231,-0.303467,0.393616];
calibx2 = [0.321594,-0.534637,-0.292175,0.466736,0.270355,0.195435,0.437439,-0.702881,-0.385529,0.043915,0.074860,-0.027832,0.670349,-0.593689,-0.112701,-0.018829,-0.079529,-0.093750,0.633789,-0.461914,0.777954,-0.059387,-0.104492,-0.319458,0.078796,-0.834167,1.733276,-0.892731,-0.418854,0.091736];
caliby2 = [-0.573151,-0.177765,0.162537,0.437317,0.380341,-0.713837,-0.530304,0.155182,0.139374,0.030579,0.437027,-0.293182,-0.469666,0.504700,0.024750,0.024628,0.678757,0.300720,-0.575623,0.633423,-0.136841,-0.385590,0.516159,0.984161,-0.999634,0.675110,-0.098938,-1.151550,-1.162682,0.892258];
calibx3 = [0.535675,-0.569092,-0.406158,0.297455,-0.053833,0.404907,0.591919,-0.733704,-0.450378,0.084930,-0.088562,0.029541,0.688843,-0.550690,0.156006,0.004028,-0.173492,-0.213928,0.778076,-0.326324,1.005707,0.130798,0.087585,-0.634338,0.247253,-0.717133,0.699707,-0.757904,0.669128,-0.714966];
caliby3 = [-0.284790,-0.311646,0.186279,0.616638,0.328278,-0.598053,-0.095520,-0.231323,0.118744,0.213226,0.222198,-0.415436,0.277252,-0.185608,-0.021423,0.152863,0.155670,-0.183807,0.393005,-0.003174,0.076660,0.031418,0.077789,0.083679,0.064026,0.040955,0.083771,-0.810211,-0.131378,0.114395];
calibx4 = [0.735535,0.577454,0.272827,-0.061829,-0.308167,-0.244507,0.389221,0.330139,-0.204224,-0.162231,-0.044250,-0.437927,-0.187622,0.160339,-0.564148,-0.112915,-0.180664,-0.797668,-0.744080,0.289398,-0.705383,0.054413,0.192688,-0.732788,-0.591187,1.066101,-0.308044,0.257690,1.339478,1.098328];
caliby4 = [-0.006561,0.194733,-0.518005,-0.281158,0.363342,-0.038544,-0.122803,0.535126,-0.608398,-0.319275,0.188354,-0.014343,-0.038513,0.946899,-0.518280,0.107117,0.265396,-0.000824,-0.377533,1.021301,-0.504425,0.885422,0.367615,0.111359,-0.474731,0.159241,-1.244476,1.294312,-1.401049,-0.238785];
calibx5 = [-0.394226,0.496613,0.322479,0.669067,-0.019135,-0.575836,-0.316040,0.212524,-0.033356,0.354889,0.050934,-0.283997,-0.185120,-0.080566,-0.478821,0.212280,-0.021179,-0.266602,0.111633,-0.209778,-0.643372,0.159424,-0.177826,-0.066528,0.468567,0.179626,0.199463,-0.026245,-0.301758,0.687927];
caliby5 = [-0.171814,0.489136,0.591705,-0.542480,-0.642792,-0.041840,-0.132233,0.227539,0.787323,-0.704163,-0.397888,-0.032043,-0.135590,-0.025452,0.961212,-0.483032,0.359436,-0.015167,0.155487,-0.626160,0.915710,-0.255524,1.183731,0.161713,0.414093,-1.086914,-0.088623,-1.017639,0.218410,-0.296860];


x1 = [-0.212769,-0.479889,0.335419,1.029358,-0.025208,-0.451111,-0.051758,-0.838318,0.338013,0.813660,-0.217163,-0.698303,1.151184,-0.685059,-0.031128,-0.265869,-0.306274,0.207703,2.205811,-0.468811,-0.414124,-1.304962,-0.040405,1.188843,-0.900330,-0.819214,1.790619,0.252747,0.217834,-1.328247];
y1 = [-0.905792,-1.405365,-0.468262,0.949371,1.325134,-0.167603,-0.546387,-0.835541,-0.536804,0.553497,1.139618,0.087463,0.475616,0.763184,-0.046814,0.135468,0.086212,-0.030457,1.022095,2.174316,0.811310,-0.913269,-1.511993,-0.341248,-1.495239,-0.924988,-0.239990,-0.522736,0.571411,0.634338];
x2 = [-0.338745,-0.766693,0.308167,1.661499,1.055725,-0.017578,-0.253357,-1.101074,-0.606049,0.675690,0.621918,-0.697693,1.121582,-0.492493,-1.326477,-0.797760,-0.259521,-0.245483,2.214111,0.401550,-0.860962,-1.951538,-1.118225,0.746887,-1.653625,-0.569336,2.894867,1.850891,-0.686615,0.247314];
y2 = [-0.611481,-0.970734,-0.505646,0.649017,1.749115,0.513580,-1.194641,-0.799316,-0.829590,-0.329407,1.366196,0.306793,-1.060272,0.837830,0.200836,-0.546539,0.485733,-0.077286,-0.380310,2.442352,1.542511,-0.461639,-1.174255,-0.183289,-0.958954,-0.303345,-0.894775,-0.100128,-1.274551,2.435318];
x3 = [0.164337,0.101288,0.946899,1.245819,-0.017670,-0.143646,-0.245483,-0.447052,0.437897,-0.011017,-0.638489,-0.674011,0.427307,-0.636169,-0.346893,-1.533661,-0.754425,0.185791,1.628967,-0.502106,-1.028839,-2.044922,0.070313,1.133362,-0.712158,-0.006470,1.616852,2.133362,1.265930,-1.377991];
y3 = [-0.969849,-1.655853,-0.390320,1.171173,1.248840,-0.142517,-0.260345,-1.292297,-0.012268,0.979401,1.097961,0.442322,0.750427,-0.258179,0.346161,0.057343,-0.552643,0.222809,1.053253,1.354675,0.529205,-1.257614,-2.359329,-0.368713,-1.662384,0.620972,-0.777832,0.603653,0.742088,0.796310];
x4 = [-0.396057,0.519836,0.939148,0.414734,-0.874329,-1.701477,-0.149780,0.976379,0.808105,0.750885,0.184631,-1.220703,0.314758,0.656799,-0.640503,0.043823,0.553894,-0.189575,0.440918,0.043549,-2.201813,-1.223022,0.718689,1.387085,-2.017883,0.774872,0.410309,0.529510,-0.210205,0.343445];
y4 = [-0.450989,-0.515381,-0.594269,0.378479,1.361145,0.436554,-0.755524,-0.120941,-0.760773,-0.063202,0.883667,0.284027,-0.091827,1.298645,-0.347839,-0.771118,-0.498230,-0.212799,0.795410,2.343384,0.114777,-0.775833,-1.764389,-0.627594,-0.343140,-1.445251,-1.030609,2.372040,1.030411,-0.118774];
x5 = [-1.138855,0.454468,1.350159,2.002869,0.408325,-1.078857,-1.254944,-0.384064,0.514191,1.140839,-0.206116,-1.111633,0.080017,-0.563599,-0.740753,-0.214233,-0.716980,-0.176819,1.913025,0.055359,-1.622467,-1.668976,-0.788147,1.366943,-0.496399,0.250183,1.196594,0.757156,0.851837,-0.023804];
y5 = [-0.300079,-0.290894,-0.093994,-0.197540,0.375854,0.667419,-0.145721,-0.427582,0.019135,-0.299438,0.369415,0.288940,0.152954,0.404114,0.862183,-0.441193,-0.278610,-0.813354,0.702606,0.980713,1.794739,-0.762573,-0.988632,-1.519379,0.136444,-2.357666,-0.190125,-0.688293,1.947342,1.064224];


subplot(1,2,1);
scatter(calibx1, caliby1, 'x', 'MarkerEdgeColor', 'b');
hold on;
scatter(calibx2, caliby2, 'x', 'MarkerEdgeColor', 'b');
hold on;
scatter(calibx3, caliby3, 'x', 'MarkerEdgeColor', 'b');
hold on;
scatter(calibx4, caliby4, 'x', 'MarkerEdgeColor', 'b');
hold on;
scatter(calibx5, caliby5, 'x', 'MarkerEdgeColor', 'b');
axis([-4 4 -4 4])
xlabel('x');
ylabel('y');
%legend('Bild 1', 'Bild 2', 'Bild 3', 'Bild 4', 'Bild 5', 'Bild 6' );

subplot(1,2,2);
scatter(x1, y1, 'x', 'MarkerEdgeColor', 'b');
hold on;
scatter(x2, y2, 'x', 'MarkerEdgeColor', 'b');
hold on;
scatter(x3, y3, 'x', 'MarkerEdgeColor', 'b');
hold on;
scatter(x4, y4, 'x', 'MarkerEdgeColor', 'b');
hold on;
scatter(x5, y5, 'x', 'MarkerEdgeColor', 'b');
axis([-4 4 -4 4])
xlabel('x');
ylabel('y');
%legend('Bild 1', 'Bild 2', 'Bild 3', 'Bild 4', 'Bild 5', 'Bild 6' );
end

