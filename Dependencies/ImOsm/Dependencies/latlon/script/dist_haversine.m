function [d,a] = dist_haversine(lat1, lon1, lat2, lon2)
    R = 6371e3;
    RAD = pi/180;
    phi1 = lat1 * RAD;
    phi2 = lat2 * RAD;
    dphi = (lat2-lat1) * RAD;
    dlam = (lon2-lon1) * RAD;
    
    a = sin(dphi/2) * sin(dphi/2) + ...
        cos(phi1) * cos(phi2) * sin(dlam/2) * sin(dlam/2);
    c = 2 * atan2(sqrt(a), sqrt(1-a));
    d = R*c;
end