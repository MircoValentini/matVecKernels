clear all;
close all;
clc;

S = load( "sparsityPattern.dat" );
N = max(S(:,2))+1;
xx = [1:N]';
ff = zeros( N,1 );
for  i = 1 : size(S,1)
  idx = S(i,2);
  con = S(i,3) - idx;
  if ( ff(idx+1,1) < con )
    ff(idx+1,1) = con;
  end
end

plot( xx, ff, '.k' );