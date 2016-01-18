function x=dreadbin(fname, varargin) 

optargin = size(varargin,2);
stdargin = nargin - optargin;
for i=optargin:-1:1
dim(optargin-i+1)=varargin{i};
end

fid = fopen(fname,'rb');   %open file
data = fread(fid, 'double');  %read in the data 'int32' 'int64'
fclose(fid);   %close file
data = reshape(data, dim);
x = permute(data,optargin:-1:1);

end