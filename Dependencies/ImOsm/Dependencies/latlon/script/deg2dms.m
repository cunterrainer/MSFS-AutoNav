function [d,m,s,t] = deg2dms(deg)
    m = (deg - floor(deg)) * 60;
    s = (m - floor(m)) * 60;
    t = (s - floor(s)) * 10;
    d = floor(deg);
    m = floor(m);
    s = floor(s);
    t = floor(t);
end