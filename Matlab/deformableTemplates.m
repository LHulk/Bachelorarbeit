function[] = deformableTemplates()
    I = imread('../img/v2_pattern/topOnlyLines.png');
    I = rgb2gray(I);
    I = imresize(I, [1000 1000]);
    %imshow(I);
    I = imgaussfilt(I, 3);
    Gmag = imgradient(I,'prewitt');
    magnitude = Gmag;

    [dx,dy] = imgradientxy(I);
    Gdir = atan2(dy,dx);

    orientation = imgaussfilt(Gdir, 7);
    magnitude = imgaussfilt(magnitude, 7);

    canny = edge(I, 'Canny', 0.5);
    %imshowpair(orientation,angleHSV(orientation,canny),'montage');

    [ddx,ddy] = imgradientxy(magnitude);
    ddx = 1/8*ddx;
    ddy = 1/8*ddy;
    ddx = imgaussfilt(ddx, 3);
    ddy = imgaussfilt(ddy, 3);
    %imshowpair(ddx,ddy,'montage');
    %figure;

    nxm = size(magnitude);

    energyFunc = @(params) e(params, magnitude, orientation);
    energyFuncDeriv = @(params) eDeriv(params, ddx, ddy);


%     samples = 50;
%     p = @(i) i/samples*2*pi;
%     ata = @(v) atan2(v(2,:),v(1,:));
%     e1 = @(params) -1/samples*sum(readMultiple(magnitude, ellipse(params, p([0:samples-1]))));
%
%     e2 = @(params) 20*1/(samples)*sum((readMultiple(orientation, ellipse(params,p([0:samples-1])))-ata(ellipse(params,p([0:samples-1])) - [params(1)*ones(1,samples);zeros(1,samples)] - [zeros(1,samples);params(2)*ones(1,samples)])).^2);
%     e = @(params) e1(params)+e2(params);


    start = nxm(1)/2 - 80;
    stop = nxm(1)/2 - 130;

    %initEllipse = [nxm(1)/2, nxm(2)/2, nxm(1)/2 - 10, nxm(2)/2 - 10, 0];
    %initEllipse = [nxm(1)/2, nxm(2)/2, nxm(1)/2 - 15, nxm(2)/2 - 10, 0];
    %initEllipse = [500, 500, 490, 490, 0]; %245, 225
    initEllipse = [500, 500, 10, 10, 0];
    
    magnitude2 = magnitude;
    magntiude2 = drawEllipse(magnitude, initEllipse);

%     h = 0.1
%     v = energyFunc(initEllipse + [h, 0, 0, 0, 0])
%     w = energyFunc(initEllipse)
%     (v - w)/h
%     v = energyFunc(initEllipse + [0, h, 0, 0, 0])
%     (v - w)/h


    %energyFunc(initEllipse)
    energyFuncDeriv(initEllipse)
    %readMultiple(orientation, ellipse(initEllipse,p([0:samples-1])))
    %bla = ata(ellipse(initEllipse,p([0:samples-1])) - [initEllipse(1)*ones(1,samples);zeros(1,samples)] - [zeros(1,samples);initEllipse(2)*ones(1,samples)])
%
    [A,B] = meshgrid(200:1:400, 200:1:400);
    X = ones(size(A))*500;
    Y = ones(size(A))*500;
    T = zeros(size(X));
    VALS = zeros(size(X));

    d = size(A);
    for i = 1:d(1)
        disp(i);
        for j = 1:d(2)
            VALS(i,j) = energyFunc([X(i,j); Y(i,j); A(i,j); B(i,j); T(i,j)]);

        end
    end

    surf(A,B,VALS);
    figure;
    return;
    

    res = zeros(size(magnitude));
    angle = angleHSV(orientation,canny);

    %options = optimoptions(@fminunc,'Display','iter','Algorithm','quasi-newton', 'TolFun', 1e-40);
    boths = @(params) bothEandEDeriv(params, magnitude, orientation, ddx, ddy);
    options = optimoptions('fminunc','Algorithm','trust-region','GradObj','on', 'Display','iter-detailed', 'HessUpdate', 'dfp', 'MaxIter', 200);

    currentEllipse = [0,0,0,0,0];
    while norm(currentEllipse - initEllipse) >= 0.01
        currentEllipse = initEllipse;
        initEllipse = fminunc(boths, initEllipse, options);
        initEllipse = [round(initEllipse(1:4)),initEllipse(5)];
        magnitude = drawEllipse(magnitude, initEllipse);
        
    end
    
    initEllipse
    
     %initEllipse = fminunc(boths, initEllipse, options)
     %initEllipse = [round(initEllipse(1:4)),initEllipse(5)];
%     initEllipse = fminunc(boths, initEllipse, options)
%     initEllipse = [round(initEllipse(1:4)),initEllipse(5)];
%     initEllipse = fminunc(boths, initEllipse, options)
%     initEllipse = [round(initEllipse(1:4)),initEllipse(5)];
    
    
    
    magnitude = drawEllipse(magnitude, initEllipse);
    imshowpair(magnitude,magntiude2, 'montage');
end


function[samples] = numSamples(params)
    sampleBias = 50;
    samples =  round(20*(params(3) + params(4))/100) + sampleBias;
end

function[val] = e1(params, magnitude)
    samples = numSamples(params);
    p = @(i) i/samples*2*pi;
    val = -1/samples*sum(readMultiple(magnitude, ellipse(params, p([0:samples-1]))));
end

function[val] = e1Deriv(params, ddx, ddy)
    samples = numSamples(params);
    p = @(i) i/samples*2*pi;
    vals = zeros(5,samples);
    cost = cos(p([0:samples-1]));
    sint = sin(p([0:samples-1]));
    vals(1,:) = readMultiple(ddx, ellipse(params, p([0:samples-1])));
    vals(2,:) = readMultiple(ddy, ellipse(params, p([0:samples-1])));
    vals(3,:) = cost.*( vals(1,:)*cos(params(5)) + vals(2,:)*sin(params(5)) );
    vals(4,:) = sint.*( vals(2,:)*cos(params(5)) - vals(1,:)*sin(params(5)) );
    vals(5,:) = vals(1,:).*(-params(3)*sin(params(5))*cost - params(4)*cos(params(5))*sint) + vals(2,:).*(params(3)*cos(params(5))*cost - params(4)*sin(params(5))*sint);

    val = zeros(5,1);
    val(1) = sum(vals(1,:));
    val(2) = sum(vals(2,:));
    val(3) = sum(vals(3,:));
    val(4) = sum(vals(4,:));
    val(5) = sum(vals(5,:));
    val(5) = mod(val(5), 2*pi);
    val = (-1/samples)*val;

end

function[val] = e2(params, orientation)
    samples = numSamples(params);
    p = @(i) i/samples*2*pi;
    ata = @(v) atan2(v(2,:),v(1,:));
    val = 50*1/(samples)*sum((readMultiple(orientation, ellipse(params,p([0:samples-1])))- ata(ellipse(params,p([0:samples-1])) - [params(1)*ones(1,samples);zeros(1,samples)] - [zeros(1,samples);params(2)*ones(1,samples)])).^2);

end

function[val] = e2Deriv(params)
    samples = numSamples(params);
    p = @(i) i/samples*2*pi;
    atan2derivx = @(x,y) -y./(x.^2+y.^2);
    atan2derivy = @(x,y) x./(x.^2+y.^2);
    vals = zeros(5,samples);
    cost = cos(p([0:samples-1]));
    sint = sin(p([0:samples-1]));
    points = ellipse(params, p([0:samples-1]));
    vals(1,:) = atan2derivx(points(1,:),points(2,:));
    vals(2,:) = atan2derivy(points(1,:),points(2,:));
    vals(3,:) = cost.*( vals(1,:)*cos(params(5)) + vals(2,:)*sin(params(5)) );
    vals(4,:) = sint.*( vals(2,:)*cos(params(5)) - vals(1,:)*sin(params(5)) );
    vals(5,:) = vals(1,:).*(-params(3)*sin(params(5))*cost - params(4)*cos(params(5))*sint) + vals(2,:).*(params(3)*cos(params(5))*cost - params(4)*sin(params(5))*sint);

    val = zeros(5,1);
    val(1) = sum(vals(1,:));
    val(2) = sum(vals(2,:));
    val(3) = sum(vals(3,:));
    val(4) = sum(vals(4,:));
    val(5) = sum(vals(5,:));
    val(5) = mod(val(5), 2*pi);
    val = (20/samples)*val;

end


function[val] = e3(params)
    val = -0.2*(params(3)+params(4));
end

function[val] = e3Deriv(params)
    val = zeros(5,1);
    val(3) = -0.2;
    val(4) = -0.2;
end

function[val] = e(params, magnitude, orientation)
     val = e1(params, magnitude) + ...
         e2(params, orientation) +...
         e3(params);
end

function[val] = eDeriv(params, ddx, ddy)
     val = e1Deriv(params,ddx,ddy) + ...
         e2Deriv(params) + ...
         e3Deriv(params);
%    val = e1Deriv(params,ddx,ddy) + e3(params);
end

function[eVal, eDerivVal] = bothEandEDeriv(params, magnitude, orientation, ddx, ddy)
    eVal = e(params, magnitude, orientation);
    eDerivVal = eDeriv(params,ddx,ddy);
end

function[hsv] = angleHSV(I, edge)
    d = size(I);
    hsv = zeros(d(1),d(2), 3);

    hsv(:,:,1) = (edge >= 0.1) .* (I(:,:)+pi)/(2*pi);
    hsv(:,:,2) = 1;
    hsv(:,:,3) = (edge >= 0.1) .* 1;

    hsv = hsv2rgb(hsv);
end


function[vals] = readMultiple(I, mat)
    s = size(mat);
    vals = zeros(1,s(2));
    for i=1:s(2)
        curr = mat(:,i);
        curr = round(curr);
        vals(i) = I(curr(2)+1,curr(1)+1);
    end
end



function[val] = ellipse(params,phi)
    x = params(1);
    y = params(2);
    a = params(3);
    b = params(4);
    theta = params(5);
    val = [x + a*cos(phi).*cos(theta) - b*sin(phi).*sin(theta); y + a*cos(phi).*sin(theta) + b*sin(phi).*cos(theta)];
end


function[I] = drawEllipse(I,params)
    phi = 0:0.001:2*pi;
    val = max(max(I));
    if val == 0
        val = 1;
    end
    s = size(I);
    for i=phi
        xy = ellipse(params, i);
        xy = round(xy);
        if (xy(2)+2 < min(s) && xy(1)+2 < min(s)) && (xy(2) > 0 && xy(1) > 0)
            I(xy(2)+2, xy(1)+1) = val;
            I(xy(2)+1, xy(1)+2) = val;
            I(xy(2)+2, xy(1)+2) = val;
            I(xy(2)+1, xy(1)+1) = val;
        end
    end
end
