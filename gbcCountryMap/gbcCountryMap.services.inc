<?php
/**
 * @file
 * Enable Bombo Today Services to get Country Map
 */

/**
 * Implements of hook_services_resources().
 */
function gbcCountryMap_services_resources(){
  $resource = array(
    'slider' => array(
      'operations' => array(
        'index' => array(
          'help'             => 'Slide to Country Map',
          'callback'         => 'gbcCountryMap_get_slider',
          'access callback'  => 'gbcCountryMap_permission',
          'access arguments' => array('view gbcCountryMap'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name'        => 'country',
              'type'        => 'int',
              'description' => '',
              'source'      => array('param' => 'country'),
              'optional'    => TRUE,
              'default'     => 0,
            ),
          ),
        ), // end index
      ), // end operations
    ), // end latest
  );

  return $resource;
}

/**
 * Callback to Country Map service
 * @return json
 */
function gbcCountryMap_get_slider($country = NULL) {
  global $base_url;
  $list     = [];
  $slider   = '';
  $response = new stdClass();


  if ( $country == NULL || is_numeric($country) ) {
    $vid = taxonomy_vocabulary_machine_name_load('marcas')->vid;
    if ($country != NULL) {
      $term_contry = $country;
    }
    else{
      $default = gbcCountryMap_get_countries();
      $term_contry = $default[0]['tid'];
    }//endif

    $qry = db_select('taxonomy_term_data', 'ttd');
    $qry->innerJoin('field_data_field_country', 'fctry', 'fctry.entity_id = ttd.tid');
    $qry->leftJoin('field_data_field_logo', 'fimg', 'fimg.entity_id = ttd.tid');
    $qry->leftJoin('field_data_field_link', 'flink', 'flink.entity_id = ttd.tid');
    $qry->condition('ttd.vid', $vid, '=');
    $qry->condition('fctry.field_country_tid', $term_contry, '=');
    $qry->fields('ttd', ['tid', 'name']);
    $qry->fields('fimg', ['field_logo_fid']);
    $qry->fields('flink', ['field_link_value']);
    $qry->orderBy('ttd.weight', 'ASC');
    $results = $qry->execute()->fetchAll();

    if (!empty($results)) {

      foreach ($results as $term) {
        // External Function
        $img = _gbcCommon_image_path($term->field_logo_fid);
        $slider .= '<li><a href="'.$term->field_link_value.'"><img src="'.$img.'"></a></li>';
      }//endforeach

      $response->success      = TRUE;
      $response->message      = "success";
      $response->data         = $slider;
    }//endif
    else{
      $response->success = TRUE;
      $response->message = "Sin resultados";
      $response->data    = "";
    }//endif

  }
  else{
    $response->success = FALSE;
    $response->message = "Invalid params";
  }//enif

  return $response;
}
