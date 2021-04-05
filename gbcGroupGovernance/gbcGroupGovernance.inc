<?php
/**
 * Return the members list.
 * @param region  
 * @return array
 */
function gbcGroupGovernance_get_members($content) {
  $list  = [];
  $lang  = _gbcCommon_language();
  $vocab = taxonomy_vocabulary_machine_name_load('directors_members');
  $vid   = $vocab->vid;

  $qry = db_select('taxonomy_term_data', 'ttd')
      ->condition('ttd.vid', $vid, '=')
      ->condition('ttd.language', $lang, '=');
  $qry->leftJoin('field_data_field_member_position', 'fmp', 'fmp.entity_id = ttd.tid');
  $qry->leftJoin('field_data_field_history_year', 'fhy', 'fhy.entity_id = ttd.tid');
  $qry->leftJoin('field_data_field_member_group_tax', 'fmg', 'fmg.entity_id = ttd.tid');
  $qry->fields('ttd', ['tid', 'name', 'description']);
  $qry->fields('fmp', ['field_member_position_value']);
  $qry->fields('fhy', ['field_history_year_value']);
  $qry->fields('fmg', ['field_member_group_tax_tid']);
  $qry->orderBy('ttd.weight', 'ASC');
  
  $results = $qry->execute()->fetchAll();

  if (!empty($results)) {
    if ($content == 'select') {
      foreach ($results as $member) {
        $list[$member->tid] = $member->name; 
      }//endforeach
    }//endif

    else if ($content == 'dropdown') {
      foreach ($results as $member) {
        $group = gbcGroupGovernance_name($member->field_member_group_tax_tid);
        $list[$group][$member->tid] = [
          'name'     => $member->name,
          'position' => $member->field_member_position_value,
          'time'     => $member->field_history_year_value,
          'desc'     => $member->description,
        ]; 
      }//endforeach
    }//endelseif

  }//endif
  return $list;
}

/**
 * Return the name to specific member list.
 * @param ID 
 * @return string
 */
function gbcGroupGovernance_name($tid) {  
  $qry = db_select('taxonomy_term_data', 'ttd')
      ->condition('ttd.tid', $tid, '=');
  $qry->fields('ttd', ['name']);
  
  $result = $qry->execute()->fetchAll();
  return $result[0]->name;
}

/**
 * Return the documents to contents.
 * @return array
 */
function gbcGroupGovernance_get_documents() {
  $docs  = [];
  $lang  = _gbcCommon_language();
  $vocab = taxonomy_vocabulary_machine_name_load('council_directors_list');
  $vid   = $vocab->vid;

  $qry = db_select('taxonomy_term_data', 'ttd')
      ->condition('ttd.vid', $vid, '=')
      ->condition('ttd.language', $lang, '=');
  $qry->leftJoin('field_data_field_document', 'fd', 'fd.entity_id = ttd.tid');
  $qry->leftJoin('field_data_field_link', 'fl', 'fl.entity_id = ttd.tid');
  $qry->condition(db_or()
        ->condition('fd.entity_type', 'taxonomy_term', '=')
        ->condition('fl.entity_type', 'taxonomy_term', '='));
  $qry->fields('ttd', ['tid', 'name']);
  $qry->fields('fd', ['field_document_fid']);
  $qry->fields('fl', ['field_link_value']);
  $qry->orderBy('weight', 'ASC');
  $results = $qry->execute()->fetchAll();

  if (!empty($results)) {
    foreach ($results as $item) {
      $url = (!empty($item->field_document_fid)) ? _gbcCommon_image_path($item->field_document_fid) : $item->field_link_value;
      $docs[] = [
        'name' => $item->name,
        'link' => (!empty($url)) ? $url : '#',
      ];
    }//endforeach
  }//endif
  return $docs;
}

/**
 * Return the documents to contents.
 * @return array
 */
function gbcGroupGovernance_get_blocks($delta) {
  $content = [];
  if ($delta == 'gbcGroupGovernance_menu') {
    $content = _gbcCommon_lateral_menu();
  }//endif

  if ($delta == 'gbcGroupGovernance_docs') {
    $content = gbcGroupGovernance_get_documents();
  }//endif

  return theme($delta, array('content' => $content));
}