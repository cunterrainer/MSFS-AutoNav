function [d,x,y] = dist_equirect(lat1, lon1, lat2, lon2)
    R = 6371e3;
    RAD = pi/180;
    phi1 = lat1 * RAD;
    phi2 = lat2 * RAD;
    dlam = (lon2-lon1) * RAD;

    x = dlam * cos((phi1+phi2)/2)*R;
    y = (phi2-phi1)*R;
    d = sqrt(x*x + y*y);
end