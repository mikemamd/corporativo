<?php
/**
 * @file
 * Enable Bombo Today Services to get Country Map
 */

/**
 * Implements of hook_services_resources().
 */
function gbcOurPlant_services_resources(){
  $resource = array(
    'plants' => array(
      'operations' => array(
        'index' => array(
          'help'             => 'Slide to plants by brands',
          'callback'         => 'gbcOurPlant_get_slider',
          'access callback'  => 'gbcOurPlant_permission',
          'access arguments' => array('view gbcOurPlant'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name'        => 'region',
              'type'        => 'int',
              'description' => '',
              'source'      => array('param' => 'region'),
              'optional'    => TRUE,
              'default'     => 0,
            ),
            array(
              'name'        => 'brand',
              'type'        => 'int',
              'description' => '',
              'source'      => array('param' => 'brand'),
              'optional'    => TRUE,
              'default'     => 0,
            ),
            array(
              'nameplants'        => 'page',
              'type'        => 'int',
              'description' => '',
              'source'      => array('param' => 'page'),
              'optional'    => TRUE,
              'default'     => 1,
            ),
          ),
        ), // end index
      ), // end operations
    ), // end latest
  );

  return $resource;
}

/**
 * Función para validación de parametros GET del servicio
 */
function gbcOurPlant_serviceFilter_validate($region = NULL, $brand = NULL) {
  $validate = [$region, $brand];

  foreach ($validate as $value) {
    if ( $value == NULL || is_numeric($value) ) {
      $success = TRUE;
    }
    else{
     $success = FALSE;
     break;
    }
  }

  return $success;
}



/**
 * Callback to Country Map service
 * @return json
 */
function gbcOurPlant_get_slider($region = NULL, $brand = NULL) {
  global $base_url;
  $response = new stdClass();
  $plants = [];
  $dataresponse = [];

  #$response->success = TRUE;
  #$response->message = "Sin resultados";
  #$response->data    = "";

  if ( ($region == NULL || is_numeric($region)) || $brand = NULL)  {

      if ($region != NULL) {
        $term_region = $region;
        $defaultbrand = gbcOurPlant_get_brand($term_region);
        #watchdog('$defaultbrand$defaultbrand', print_r($defaultbrand,TRUE));
        $response->success      = TRUE;
        $response->message      = "success brand of region";
        $response->data = $defaultbrand;
      }

  }else{
        $response->success = FALSE;
        $response->message = "Invalid params";
        #$response->data     = "";
  }//enif

  if ( ($region != NULL || is_numeric($region)) || ($brand == NULL || is_numeric($brand)))  {

    if ($brand != NULL) {
      $term_region = $region;
      $term_brand = $brand;
      #$defaultbrand = gbcOurPlant_get_brand($term_region, $term_brand);
      $defaultplant = gbcOurPlant_get_plants($term_brand);
      $response->success      = TRUE;
      $response->message      = "success plants";
      $response->data         = $defaultplant;
    }

  }else{
        $response->success = FALSE;
        $response->message = "Invalid params";
        $response->data     = "Invalid";
  }//enif


  return $response;
}


function gbcOurPlant_get_plants($term_brand){

$plantsdata = [];

  if (empty($term_brand)) {
    return $plantsdata;
  } // end if

    $vidplant  = taxonomy_vocabulary_machine_name_load('brands_our_plants')->vid;
    #$lang = _gbcCommon_language();
    $lang = 'es';

    $qry = db_select('taxonomy_term_data', 'ttd');
    $qry->leftJoin('field_data_field_our_brand', 'fdfob', 'fdfob.entity_id = ttd.tid');
    $qry->leftJoin('field_data_field_location', 'fdfl', 'fdfl.entity_id = ttd.tid');
    $qry->leftJoin('field_data_field_email_plant', 'fdfep', 'fdfep.entity_id = ttd.tid');
    $qry->condition('ttd.vid', $vidplant, '=');
    $qry->condition('ttd.language', $lang, '=');
    $qry->condition('fdfob.field_our_brand_tid', $term_brand, '=');
    $qry->fields('fdfob', ['entity_id', 'field_our_brand_tid']);
    $qry->fields('ttd', ['tid', 'name']);
    $qry->fields('fdfl', ['field_location_value']);
    $qry->fields('fdfep', ['field_email_plant_value']);
    $qry->orderBy('ttd.weight', 'ASC');
    $results = $qry->execute()->fetchAll();

    if (isset($results) && !empty($results)) {

      foreach ($results as $term) {
        $plantsdata[] = [
          'tidplant' => $term->tid,
          'nameplant' => $term->name,
          'locationplant' => $term->field_location_value,
          'emailplant' => $term->field_email_plant_value,
          ];
      }//endforeach

    }//endif
    return $plantsdata;
  }
