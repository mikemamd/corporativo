<?php
/**
 * @file
 * Admin page callbacks to gbcSidebar module.
 */

/**
 * Administer form to recipes header banner
 *
*/
function gbcSidebar_admin_form() {
  $form  = [];
  $iframe = variable_get('gbcSidebar_iframe_yt','');

  $calc_en = variable_get('gbcSidebar_calc_en_yt','');
  $calc_es = variable_get('gbcSidebar_calc_es_yt','');

  #$Factsheet_en = variable_get('gbcSidebar_Factsheet_en_yt','');
  #$Factsheet_es = variable_get('gbcSidebar_Factsheet_es_yt','');


  $form['gbcSidebar'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Configuracion general del Sidebar'),
    '#weight'      => 1,
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE
  );
  $form['gbcSidebar']['gbcSidebar_iframe_yt'] = array(
    '#type'          => 'text_format',
    '#title'         => t( 'Iframe Sidebar' ),
    '#default_value' => !empty($iframe) ? $iframe['value'] : '' ,
    '#rows'          => 1,
    '#format'        => 'plain_text',
  );

  $form['gbcSidebar']['gbcSidebar_calc_en_yt'] = array(
    '#type'          => 'textfield',
    '#title'         => t( 'Calculator node link "EN"'),
    '#default_value' => !empty($calc_en) ? $calc_en : '' ,
  );

  $form['gbcSidebar']['gbcSidebar_calc_es_yt'] = array(
    '#type'          => 'textfield',
    '#title'         => t( 'Calculator node link "ES"'),
    '#default_value' => !empty($calc_es) ? $calc_es : '' ,
  );
/*
  $form['gbcSidebar']['gbcSidebar_Factsheet_en_yt'] = array(
    '#type'          => 'textfield',
    '#title'         => t( 'Factsheet node link "EN"'),
    '#default_value' => !empty($Factsheet_en) ? $Factsheet_en : '' ,
  );

  $form['gbcSidebar']['gbcSidebar_Factsheet_es_yt'] = array(
    '#type'          => 'textfield',
    '#title'         => t( 'Factsheet node link "ES"'),
    '#default_value' => !empty($Factsheet_es) ? $Factsheet_es : '' ,
  );
  */
  return system_settings_form($form);
}
