function [] = ransac2()

t = 0:0.01:0.6;
a = [0.356801,0.365250,0.174358,0.266906,0.230372,0.527451,0.499691,0.154352,0.192787,0.190891,0.500996,0.310696,0.299533,0.485582,0.390353,0.210075,0.418094,0.301195,0.373893,0.335340,0.206728,0.507793,0.281708,0.167664,0.438321,0.403488,0.358193,0.360914,0.664213,0.309983,0.265173,0.369336,0.067129,0.358530,0.542361,0.395512,0.226365,0.466145,0.548831,0.379167,0.531324,0.345006,0.490111,0.334187,0.295239,0.279616,0.714532,0.257156,0.382464,0.366117,0.091073,113.141859,113.342907,112.942935,113.033587,113.006744,113.172696,113.258128,113.073449,112.914446,112.863618,];
b = [0.059700,3.498674,8.624170,11.657740,13.575819,16.497695,19.743514,21.498037,27.303116,28.217299,31.397217,32.503595,36.077798,37.410405,38.162790,41.617670,45.245478,44.992321,46.116223,51.733347,49.154789,51.250147,55.478964,54.974637,56.570277,57.022863,59.600686,60.664646,62.160000,64.720077,65.621394,65.595147,66.770704,68.744439,69.566401,70.694093,71.693661,72.947124,73.500300,74.434369,75.272586,76.533195,77.935322,78.380773,79.242467,80.414487,81.254594,81.848773,82.716517,83.746588,84.583598,85.214354,85.881485,87.089984,87.665581,88.563575,88.931638,90.045871,90.555380,91.214920,92.078146,];

figure;
scatter(t, a, '+', 'MarkerEdgeColor', 'r','LineWidth', 2.5);
hold on;
scatter(t, b, '+', 'MarkerEdgeColor', 'b','LineWidth', 2.5);
legend('RANSAC', 'LSQ');
axis([-0.01 0.61 0 160])
xlabel('relativer Fehleranteil');
ylabel('Abweichung');

end
