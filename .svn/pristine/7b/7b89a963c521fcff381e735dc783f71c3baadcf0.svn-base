% example
N1=4; N2=13; N3=3; N4=16;

A=dreadbin('A.bin',N1,N2,N3,N4);
Acsv=load('A.csv');
error1=0;
error2=0;
for i=0:N1-1
    for j=0:N2-1
        for k=0:N3-1
            for l=0:N4-1
                error1=error1 + (A(i+1,j+1,k+1,l+1)~=i*1000000 + j*10000 + k*100 + l);
                error2=error2 + (Acsv(i*N2*N3*N4+j*N3*N4+k*N4+l+1)~=i*1000000 + j*10000 + k*100 + l);
            end
        end
    end
end

if (error1==0) fprintf('A.bin is correct\n');
else fprintf('error in A.bin\n');
end

if (error2==0) fprintf('A.csv is correct\n');
else fprintf('error in A.csv\n');
end
