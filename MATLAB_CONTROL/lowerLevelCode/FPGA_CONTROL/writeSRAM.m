function returnString = writeSRAM( fileHandle, colSel, data_DL, DL_nums, startStringIn )

%     data_DL is a binary(double) column vector of length 1008
%         at the end of this function if you look at any DL_nums selected DLs( at a colSel selected BL) you should see data_DL stored in the cells 
%             - data_DL(1,1) -> stored in the cell on the 1st WWL
%             - data_DL(2,1) -> stored in the cell on the 2nd WWL
%     DL_nums contains the subset of DL_0:149 to write data_DL to
%     colSel contains which bitlines to write to BL 0:7
    st                          = dbstack ;
    fnName                      = st.name ;
    zeroes                      = @( x ) zeros( x ) ;
    stringCurr                  = startStringIn ;

%     if( ~isempty( find( ( 0 <= DL_nums ) | ( DL_nums < 150 ), 1 ) ) )
%         
%         if( numel( data_DL ) == 1008 )
% 
%             data_DL             = reshape( data_DL, [ 1 numel( data_DL ) ] ) ;
%             row_indices         = 0 : 1007 ;
%             data_1_row_indices  = row_indices( logical(   data_DL ) ) ;
%             data_0_row_indices  = row_indices( logical( 1-data_DL ) ) ;
%             mask                = zeroes( [ 1 150 ] ) ;
%             mask( DL_nums + 1 ) =                   1 ; % unmasking only the chosen DLs
% 
%             %% First writing the ones            
%             data                = zeroes( [ 1 150 ] ) ; % data get's scanned into W_SC
%             data( DL_nums + 1 ) =                   1 ; % writing 1s only onto chosen DLs
%             for rowSel = data_1_row_indices
% 
%                 [ stringCurr, wlstatus, wscstatus ] ...
%                                 = perform_WxOnRowColSel_FPGA( fileHandle, rowSel, colSel, data, mask, wlstatus, wscstatus, varsIn, stringCurr ) ;
% 
%             end
% 
%             %% Next writing the zeroes
% 
%             data                = zeroes( [ 1 150 ] ) ;
%             for rowSel = data_0_row_indices
%                 [ stringCurr, wlstatus, wscstatus ] ...
%                                 = perform_WxOnRowColSel_FPGA( fileHandle, rowSel, colSel, data, mask, wlstatus, wscstatus, varsIn, stringCurr ) ;
%             end        
%             returnString = stringCurr ;
% 
%         else
% 
%             display( [ 'ERROR IN ' fnName ' length(data) is not 1008!'] );
% 
%         end
%     else
%         display( [ 'ERROR IN ' fnName ' DL_nums out of bounds!'] );
%     end

    %wlstatus                        = -1                  ;
    
    %wscstatus.data                  = zeroes( [ 1 97 ] ) ;
    %wscstatus.mask                  = zeroes( [ 1 97 ] ) ;
    %wscstatus.mask( DL_nums + 1 )   = 1                   ;
    
    data_DL        = permute(data_DL,[2,1]);  %1 x (1092*97) 
    %
    %data_DL                         = reshape( data_DL, [ numel( data_DL ) 1 ] ) ;
    data_DL         =  reshape(data_DL,1,[]);
    arguments.data_DL               = data_DL ;
    DL_nums                         = reshape( DL_nums, [ 1 numel( DL_nums ) ] ) ;
    arguments.DL_nums               = DL_nums ;
    colSel                          = reshape( colSel,  [ 1 numel( colSel  ) ] ) ;
    %for colSel_ite = colSel 
        arguments.colSel            = colSel;%colSel_ite  ;
        uart_encrypt( fileHandle, fnName, arguments ) ;
        %array_data( :, 1 + DL_nums*4 + colSel_ite ) = repmat( data_DL , [ 1 size(1 + DL_nums*4 + colSel_ite, 2) ] ) ;
    %end
    %array_data=0;
    returnString        = stringCurr ;
display( mfilename )
end