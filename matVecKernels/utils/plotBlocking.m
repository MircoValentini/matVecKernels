close allclear allclcB = load( "BandWidthBLocking.dat" );S = load( "sparsityPattern.dat" );A = sparse( S(:,2)+1, S(:,3)+1, S(:,4) );figure;hold onspy(A','r')spy(A)for i = 1 : size(B,1)  plot( [B(i,1), B(i,1), B(i,2),B(i,2), B(i,1)], [B(i,1),B(i,2),B(i,2),B(i,1),B(i,1)], '-k', 'linewidth', 2 );endforaxis equal;axis off;