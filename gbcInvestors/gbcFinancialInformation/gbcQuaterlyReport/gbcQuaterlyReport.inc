<?php
/**
 * @file
 * Costruct content to Factsheet section
 */

/**
 * Return items to factsheet
 * @return array 
 */ 
function gbcQuaterlyReport_get_items($type = NULL, $year = NULL, $quaterly = NULL, $page = NULL) {
  $response = new stdClass();
  $response->success = FALSE;

  $validate = gbcQuaterlyReport_val_params($type, $year, $quaterly, $page);

  if ($validate === FALSE ) {  
    $response->message = 'Invalid Params';
    return $response;
  } //endif

  $qry = db_select('node', 'n');
  $qry->leftJoin('field_data_field_document_adjunt', 'fda', 'fda.entity_id = n.nid');
  $qry->leftJoin('field_data_field_history_year', 'fhy', 'fhy.entity_id = n.nid');
  $qry->leftJoin('field_data_field_quarterly', 'fq', 'fq.entity_id = n.nid');
  $qry->leftJoin('field_data_field_report_category', 'fc', 'fc.entity_id = n.nid');
  $qry->leftJoin('taxonomy_term_data', 'ttd', 'ttd.tid = fq.field_quarterly_tid'); 
  $qry->condition('type', 'inv_quarterly_report', '=')
      ->condition('status', '1', '=');
  if ( $type !== NULL ) {
    $qry->condition('fc.field_report_category_tid', $type, '=');
  } // endif
  if ( $year !== NULL ) {
    $qry->condition('fhy.field_history_year_value', $year, '=');
  } // endif
  if ( $quaterly !== NULL ) {
    $qry->condition('fq.field_quarterly_tid', $quarter, '=');
  } // endif
  $qry->fields('n', ['nid', 'title'])
      ->fields('fda', ['field_document_adjunt_fid'])
      ->fields('fhy', ['field_history_year_value'])
      ->fields('fq', ['field_quarterly_tid'])
      ->fields('fc', ['field_report_category_tid'])
      ->fields('ttd', ['name']);     
  $qry->extend('PagerDefault')
      ->element('0')
      ->limit(10)
      ->orderBy('n.changed', 'ASC')
      ->execute();

  $results = $qry->execute()->fetchAll();

  if ( !empty($results) ) {
    $total = 0;
    foreach ($results as $result) {
      $file = _gbcCommon_image_path($result->field_document_adjunt_fid);
      $opt  = [ 'html' => TRUE, 'attributes' => ['target'=>'_blank'], ];
      $link = l('<i class="icon-download"></i><span>'.$result->title.'</span>', $file, $opt);

      $content[] = [
        'id'      => $result->nid,
        'name'    => $result->title,
        'type'    => $result->field_report_category_tid,        
        'year'    => $result->field_history_year_value,
        'quarter' => $result->name,
        'file'    => $link,
      ];
      ++$total;
    } //endforeach 
    $response->success      = TRUE;
    $response->message      = t("success");
    $response->totalResults = $total;
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
function gbcQuaterlyReport_val_params($type, $year, $quaterly, $page){
  $success  = TRUE;
  $validate = [$type, $year, $quaterly, $page];
  
  if ($year !== NULL) {
    $long_year = strlen($year);
    
    if ( $long_year != 4 ) {
      $success = FALSE;
      return $success;
    } // endif
  }// endif

  foreach ($validate as $value) {
    if ( $value === NULL || is_numeric($value) ) {
      $success = TRUE;
    }
    else{
     $success = FALSE;
     return $success;
    } //endif
  } //endforeach

  return $success;
}

/**
 * Return items to vocabulary of quaterly report type
 * @return array 
 */ 
function gbcQuaterlyReport_get_type() {
  $options = [];

  $lang  = _gbcCommon_language();
  $vocab = taxonomy_vocabulary_machine_name_load('quarterly_report_category');
  $vid   = $vocab->vid;

  $qry = db_select('taxonomy_term_data', 'ttd');
  $qry->condition('ttd.vid', $vid, '=')
      ->condition('ttd.language', $lang, '=');
  $qry->fields('ttd', ['tid', 'name'])
      ->orderBy('ttd.weight', 'ASC');
  $results = $qry->execute()->fetchAll();

  if (isset($results) && !empty($results)) {
    foreach ($results as $result) {  
      $options[$result->tid] = $result->name;
    }// endforeach
  }// endoif
  
  return $options;
}
