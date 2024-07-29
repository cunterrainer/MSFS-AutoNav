function d = dist_flatearth(lat1, lon1, lat2, lon2)
    R = 6371e3;
    RAD = pi/180;
    phi1 = lat1 * RAD;
    phi2 = lat2 * RAD;
    dlam = (lon2-lon1) * RAD;

    th1 = pi/2-phi1;
    th2 = pi/2-phi2;
    d = sqrt(th1*th1 + th2*th2 - 2*th1*th1*cos(dlam))*R;
end