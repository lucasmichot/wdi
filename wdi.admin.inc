<?php
// $Id$

/**
 * @file
 * Administrative page callbacks for the wdi module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function wdi_admin_settings() {
  $form = array();

  $form['wdi_application_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Application name'),
    '#description' => t('The name of the shortcut. If missing, the document title is used instead.'),
    '#default_value' => variable_get('wdi_application_name', variable_get('site_name')),
  );

  $form['wdi_msapplication_tooltip'] = array(
    '#type' => 'textfield',
    '#title' => t('Application tooltip'),
    '#description' => t('Optional text that is displayed as a tooltip when the mouse pointer hovers over the pinned site shortcut icon in the Windows Start menu or desktop.'),
    '#default_value' => variable_get('wdi_msapplication_tooltip', variable_get('site_slogan')),
  );

  $form['wdi_msapplication_starturl'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Application start URL'),
    '#description' => t('The root URL of the application. If missing, the address of the current page is used instead.'),
    '#default_value' => variable_get('wdi_msapplication_starturl', variable_get('site_frontpage')),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? NULL : '?q='),
  );

  $form['wdi_msapplication_navbutton_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Navigation buttons color'),
    '#description' => t('The color of the Back and Forward buttons in the pinned site browser window. Any named color, or hex color value as defined by Cascading Style Sheets (CSS), Level 3 (CSS3) is valid. If this meta element is absent, the color is based on the shortcut icon.'),
    '#default_value' => variable_get('wdi_msapplication_navbutton_color'),
  );

  $form['wdi_msapplication_window_width'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 4,
    '#title' => t('Window width'),
    '#description' => t('The initial width of the pinned site browser window. Minimum is 800. Note that user action overwrites this value.'),
    '#default_value' => variable_get('wdi_msapplication_window_width', 800),
    '#field_suffix' => t('px'),
  );

  $form['wdi_msapplication_window_height'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 4,
    '#title' => t('Window height'),
    '#description' => t('The initial height of the pinned site browser window. Minimum is 800. Note that user action overwrites this value.'),
    '#default_value' => variable_get('wdi_msapplication_window_height', 600),
    '#field_suffix' => t('px'),
  );

  $form = system_settings_form($form);

  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('wdi_admin_settings_reset_submit'),
  );

  return $form;
}

