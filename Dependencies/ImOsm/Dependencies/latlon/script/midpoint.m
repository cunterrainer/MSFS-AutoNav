function [lat, lon] = midpoint(lat1, lon1, lat2, lon2)
    RAD = pi/180;
    DEG = 180/pi;
    phi1 = lat1 * RAD;
    phi2 = lat2 * RAD;
    lam1 = lon1 * RAD;
    dlam = (lon2-lon1) * RAD;

    Bx = cos(phi2) * cos(dlam);
    By = cos(phi2) * sin(dlam);
    y = sin(phi1)+sin(phi2);
    x = sqrt((cos(phi1)+Bx)*(cos(phi1)+Bx) + By*By);
    phi3 = atan2(y,x);
    lam3 = lam1 + atan2(By, cos(phi1) + Bx);

    lat = phi3 * DEG;
    lon = lam3 * DEG;
    lon = mod((lon+540), 360)-180;
end