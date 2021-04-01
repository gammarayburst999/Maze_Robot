function  print_function(filname,matrix_hor_wall,matrix_ver_wall,po,dir)
char_h='-';
char_v='|';
char_space=' ';
flag=0;
complete_maze=strings(11,37);
% matrix_hor_wall=ones(4,9);
% matrix_ver_wall=ones(5,8);
[hrow_02, hcol_02]=size(matrix_hor_wall);
[vrow_02, vcol_02]=size(matrix_ver_wall);
 for i4=2:11
 for i5=2:37
     complete_maze(i4,i5)= char_space;
 end
 end



for ro_01=1:hrow_02
    for vco_01=1:hcol_02
        val_01= matrix_hor_wall(ro_01,vco_01);
       vv_01=4*vco_01-4;
        rr_01=2*(ro_01)+1;
        if(val_01==1)
        complete_maze(rr_01,vv_01+1)= char_space;
        complete_maze(rr_01,vv_01+2)= char_h;
        complete_maze(rr_01,vv_01+3)= char_h;
        complete_maze(rr_01,vv_01+4)=char_h;    
        end
        if(val_01==0)
        complete_maze(rr_01,vv_01+1)= char_space;
        complete_maze(rr_01,vv_01+2)= char_space;
        complete_maze(rr_01,vv_01+3)= char_space;
        complete_maze(rr_01,vv_01+4)=char_space;  
        end   
    end
end

for ro_02=1:vrow_02
    for vco_02=1:vcol_02
        val_02= matrix_ver_wall(ro_02,vco_02);
        vv=4*(vco_02)-3;
        rr=2*ro_02;
        if(val_02==1)
        complete_maze(rr,vv+1)= char_space;
        complete_maze(rr,vv+2)= char_space;
        complete_maze(rr,vv+3)= char_space;
        complete_maze(rr,vv+4)=char_v;    
        end
        if(val_02==0)
        complete_maze(rr,vv+1)= char_space;
        complete_maze(rr,vv+2)= char_space;
        complete_maze(rr,vv+3)= char_space;
        complete_maze(rr,vv+4)=char_space;  
        flag=vv+4;
        end   
        if(flag==33)
        complete_maze(rr,flag+1)= char_space;
        complete_maze(rr,flag+2)= char_space;
        complete_maze(rr,flag+3)= char_space;  
        complete_maze(rr,flag+4)=char_space;
        end
    end
end

for i3=1:33
    if(mod((i3-1),4)==0)
        complete_maze(1,i3)= char_space;
        complete_maze(1,i3+1)= char_h;
        complete_maze(1,i3+2)= char_h;
        complete_maze(1,i3+3)= char_h;
        
        complete_maze(11,i3)= char_space;
        complete_maze(11,i3+1)= char_h;
        complete_maze(11,i3+2)= char_h;
        complete_maze(11,i3+3)= char_h;
        %complete_maze(11,i3)= [char_space,char_h];
    end
    if(i3<12)
        complete_maze(i3,1)=' ';
        complete_maze(i3,37)=' ';
        
        if(mod(i3,2)==0)
            complete_maze(i3,1)='|';
            complete_maze(i3,37)='|';
        end
    end
end

if(po(1)==0&&po(2)==0)
complete_maze(2,3)=dir;
end

if(po(1)==0&&po(2)==1)

complete_maze(10,3)=dir;
end
if(po(1)==1&&po(2)==0)
complete_maze(2,35)=dir;
end
if(po(1)==1&&po(2)==1)
complete_maze(10,35)=dir;
end
% for i4=2:10
% for i5=2:36
%     complete_maze(i4,i5)= char_space;
% end
% end

% 
 matrix = complete_maze; % example matrix
[rows, ncols] = size(matrix);
mrows=1;
%outputstr = ['%' num2str(mrows) 'C']; % template for the string, you put your datatype here
outputstr = ['%' num2str(mrows) 'c'];
outputstr = repmat(outputstr, 1, ncols); % replicate it to match the number of columns
outputstr = [outputstr '\n']; % add a new line if you want
fid = fopen(filname,'wt');
fprintf(fid,outputstr, matrix.'); % write it
fclose(fid);
end
