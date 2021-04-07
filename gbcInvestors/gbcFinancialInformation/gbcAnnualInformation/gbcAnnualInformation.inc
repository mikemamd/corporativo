<?php
/**
 * @file
 * Costruct content to Annual Information section
 */

/**
 * Return items to factsheet
 * @return array 
 */ 
function gbcAnnualInformation_get_items($year = NULL) {
  $content = [];
  $response = new stdClass();
  $response->success = FALSE;

  $validate = gbcAnnualInformation_val_params($year);

  if ($validate === FALSE ) {  
    $response->message = 'Invalid Params';
    return $response;
  } //endif

  $qry = db_select('node', 'n');
  $qry->leftJoin('field_data_field_document_adjunt', 'fda', 'fda.entity_id = n.nid');
  $qry->leftJoin('field_data_field_document', 'fd', 'fd.entity_id = n.nid');
  $qry->leftJoin('field_data_field_history_year', 'fhy', 'fhy.entity_id = n.nid');
  $qry->condition('type', 'inv_anual_report', '=')
      ->condition('status', '1', '=');
  if ( $year !== NULL ) {
    $qry->condition('fhy.field_history_year_value', $year, '=');
  } // endif
  $qry->fields('n', ['nid', 'title'])
      ->fields('fda', ['field_document_adjunt_fid'])
      ->fields('fd', ['field_document_fid', 'field_document_fid'])
      ->fields('fhy', ['field_history_year_value']);
  $qry->range(0,1)
      ->orderBy('n.changed', 'ASC')
      ->execute();

  $results = $qry->execute()->fetchAll();

  if ( !empty($results) ) {
    foreach ($results as $result) {
      $opt  = [ 'html' => TRUE, 'attributes' => ['target'=>'_blank'], ];

      $doc_path = _gbcCommon_image_path($result->field_document_adjunt_fid);
      $doc_link = l('<i class="icon-download"></i><span>'.t('Resumen de Informe Anual').'</span>', $doc_path, $opt);

      $rep_path = _gbcCommon_image_path($result->field_document_fid);
      $rep_link = l('<i class="icon-download"></i><span>'.$result->title.'</span> ', $rep_path, $opt);

      $content = [
        'id'       => $result->nid,
        'name'     => $result->title,
        'year'     => $result->field_history_year_value,
        'report'   => $rep_link,
        'document' => $doc_link,
      ];
    } //endforeach 
    $response->success      = TRUE;
    $response->message      = t("success");
    $response->data         = $content;
  }
  else {
    $response->success = TRUE;
    $response->message = t("Filtrado sin resultados"); 
    $response->data    = $content;
  } // endif

  return $response;
}

/**
 * Validate VIN
 * @return boolean
 */ 
function gbcAnnualInformation_val_params($year){
  $success  = TRUE;
  
  if ($year !== NULL) {
    $long_year = strlen($year);
    
    if ( $long_year != 4 || !is_numeric($value) ) {
      $success = FALSE;
      return $success;
    } // endif
  }// endif

  return $success;
}
