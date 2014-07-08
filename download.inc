<?php
	function download_form($form, &$form_state){
		
		global $user;
		$path = drupal_get_path('module', 'foss_upload');
		drupal_add_js($path.'/filter.js');
			
		$options_first = _foss_dropdown_options();
		$selected = isset($form_state['values']['foss']) ? $form_state['values']['foss'] : key($options_first);
		$form['foss'] = array(
			'#title' => t('FOSS Category'),
			'#type' => 'select',
			'#options' => $options_first,
			'#default_value' => $selected,
			'#ajax' => array(
			'callback' => 'doctype_dropdown_callback',
			'wrapper' => 'dropdown-second-replace',
			),
		);
		$form['files'] = array(
			'#type' => 'select',
			'#title' => t('File to be Uploaded'),
			'#multiple' => true,
			'#options' => _doc_dropdown_options($selected),
			'#prefix' => '<div id="dropdown-second-replace">',
			'#suffix' => '</div>',
			'#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : '',
		);
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Email'),
			'#required' => TRUE,
			'#default_value' => $user->mail,
			'#description' => "Please enter your email.",
			'#size' => 20,
			'#maxlength' => 20,
			'#element_validate' => array('download_email_element_validate'),
		);
		$form['submit'] = array(
			'#value' => 'Submit',
			'#type' => 'submit',
		);
	return $form;
	}
	
	function _foss_dropdown_options() {
		return drupal_map_assoc(
				array(
					t('Scilab'),
					t('Python'),
					t('Oscad'),
					t('OpenFOAM'),
					t('OR-Tools')
				)
		);
	}
	
	function doctype_dropdown_callback($form, $form_state) {
		return $form['files'];
	}
	
	function _doc_dropdown_options($key = '') {
		$options = array();
		$query = "select DISTINCT doc_type from foss_documents WHERE foss = '".$key."' GROUP BY doc_type ASC";
		$result = db_query($query);
		foreach ($result as $record) {
			$options[$record->doc_type] = $record->doc_type;
		}
		return $options;
	}
	
	function download_email_element_validate($element, &$form_state, $form) {
		if (!valid_email_address($element['#value'])) {
			form_error($element, t('Please enter a valid email address.'));
		}
	}
	
	function download_form_submit($form,&$form_state){
		global $user;
		$selected_file = "";
		
		foreach($form_state['values']['files'] as $file){
			if($form_state['values']['files'])
			{
				$selected_file .= $file.",";
			}
			else
			{
				$selected_file .= $file;
			}
		}
		$query = "insert into download_file (foss,files,user_mail) values('".$form_state['values']['foss']."','".$selected_file."','".$form_state['values']['email']."')";
		db_query($query);
		drupal_set_message("Inserted successfully");
		$doc = array();
		$doc = explode(",",$selected_file);
		$link = array();
		$links_doc = '';
		foreach($doc as $a)
		{
			$result = db_query("select doc_url from foss_documents where foss = '".$form_state['values']['foss']."' and doc_type = '".$a."'");
			foreach ($result as $r)
			{
				$link[$a] .= $r->doc_url;
			}
		}
		
		foreach($link as $key=>$url){
			//$links_doc .= '<a href="' . $url . '">' . $form_state['values']['foss'] ." ". $key . '</a><br>';
			$links_doc .= l($form_state['values']['foss'] ." ". $key, $url, array('absolute' => TRUE))."<br>";
		}
		
		//echo $links_doc;
		//die;
		$mailto = variable_get('request_notification_emails', '');
		$subject = 'Request Notification';
		$message[] ="Following are the links<br>".$links_doc;
		request_notify($mailto, $subject, $message);
	}
	
?>
