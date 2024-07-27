function [b,x,y] = bearing(lat1, lon1, lat2, lon2)
    RAD = pi/180;
    DEG = 180/pi;
    phi1 = lat1 * RAD;
    phi2 = lat2 * RAD;
    dlam = (lon2-lon1) * RAD;

    x = cos(phi1)*sin(phi2) - sin(phi1)*cos(phi2)*cos(dlam);
    y = sin(dlam)*cos(phi2);
    th = atan2(y,x);
    b = mod((th*DEG + 360), 360);
end