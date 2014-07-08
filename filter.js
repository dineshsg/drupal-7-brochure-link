jQuery(function($) {
	webroot = 'http://10.118.248.101/testing/';
	loading_image = "<img src='http://10.118.248.101/spoken_tutorial_org/ajax-loader.gif' />";
	$('.foss_category').on("change", function(){
		if($('.foss_category').val() != ''){
			$.ajax({
				type : "POST",
				url : webroot + "filter/get_doc_type",
				data : {
					'foss_cat' : $('.foss_category').val()
				},
				beforeSend: function() {
					field_data = $('.doc_type').html();
					$('.doc_type').html(loading_image);
				},
				success : function(json){
					$('.doc_type').html(field_data);
					output = JSON.parse(json);
					if(output){
						html_data = "";
						count = 0;
						for (var i=0; i < output.length; i++) {
							html_data += "<option value='"+ output[i][0] +"'>" + output[i][0] + "</option>\n";
							count++;
						}
						if(count > 1) {
							html_data = "<option value=''>Select Language</option>\n"+html_data;
						}
						$('.doc_type').html(html_data);
					}else{
						alert('Error fetching languages, please refresh the page and try again');
					}
				}
			});
		}
	});
}); 

