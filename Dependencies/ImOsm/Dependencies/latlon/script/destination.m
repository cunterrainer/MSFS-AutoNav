function [lat2,lon2] = destination(lat1, lon1, d, b)
    R = 6371e3;
    RAD = pi/180;
    DEG = 180/pi;
    phi1 = lat1 * RAD;
    lam1 = lon1 * RAD;
    th = b * RAD;
    delta = d/R;
    
    sin_phi2 = sin(phi1) * cos(delta) + cos(phi1)*sin(delta)*cos(th);
    phi2 = asin(sin_phi2);
    y = sin(th)*sin(delta)*cos(phi1);
    x = cos(delta)-sin(phi1)*sin_phi2;
    lam2 = lam1 + atan2(y,x);

    lat2 = phi2 * DEG;
    lon2 = lam2 * DEG;
    lon2 = mod((lon2+540), 360)-180;
end