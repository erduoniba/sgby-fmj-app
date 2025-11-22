$(document).ready(function(){
//	//点击改变底部导航栏背景样式
//	$('footer ul li').click(function(){
//		var index=$(this).index();
//		$('footer ul li').eq(index).css({
//			background:'linear-gradient(#CBCBCB,#999999)'
//		}).siblings().css({
//			background:'linear-gradient(#F0F0F0,#CBCBCB)'
//		})
//	})
    $('body').click(function(e){
    	
	})


     $('#biankuang').click(function(){
     	var bor=$('#huabu ul li').css("border-color")
     	if(bor=='rgb(54, 111, 149)'){
     		$('#huabu ul li').css({
				border:'1px solid white'
			})
     	}else{
     		$('#huabu ul li').css({
				border:'1px solid #366F95'
			})
     	}
		
	});
	
	$('#shengcheng').click(function(){
		$("#shengcheng").attr("disabled", true); 
	});
})