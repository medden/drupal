<?php

/**
 * @file
 *   Administrative pages for the Domain 301 Redirect module.
 */

/**
 * Admin form for configuring Domain 301 Redirect.
 */
function domain_301_redirect_admin_form($form_state) {
  $form['domain_301_redirect_enabled'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#description' => t('Enable this setting to start 301 redirects to the domain below for all other domains.'),
      '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
      '#default_value' => variable_get('domain_301_redirect_enabled', 0),
  );
  $form['domain_301_redirect_domain'] = array(
      '#type' => 'textfield',
      '#title' => t('Domain'),
      '#description' => t('This is the domain that all other domains that point to this site will be 301 redirected to. This value should also include the scheme (http or https). E.g. http://www.testsite.com'),
      '#default_value' => variable_get('domain_301_redirect_domain', ''),
  );
  $form['domain_301_redirect_check_period'] = array(
      '#type' => 'select',
      '#title' => t('Domain Check'),
      '#description' => t('This option selects the period between domain validation checks. If the Domain no longer points to this site, Domain 301 Redirection will be disabled.'),
      '#options' => array(0 => t('Disabled'), 3600 => t('1 hour'), 7200 => t('2 hours'), 10800 => t('3 hours'), 21600 => t('6 hours'), 43200 => t('12 hours'), 86400 => t('1 day')),
      '#default_value' => variable_get('domain_301_redirect_check_period', 60 * 60 * 3), // default of 3 horus
  );
  return system_settings_form($form);
}

/**
 * Validation hook for Domain 301 Redirect Admin page.
 */
function domain_301_redirect_admin_form_validate(&$form, &$form_state) {
  if (!empty($form_state['values']['domain_301_redirect_enabled'])) {
    if (empty($form_state['values']['domain_301_redirect_domain'])) {
      form_set_error('domain_301_redirect_enabled', 'Domain 301 redirection can not be enabled if no domain is set.');
    }
    else {
      $domain = $form_state['values']['domain_301_redirect_domain'];
      if (!preg_match('|^https?://|', $domain)) {
        $domain = 'http://' . $domain;
      }
      $domain_parts = parse_url($domain);
      $domain = $domain_parts['scheme'] . '://' . $domain_parts['host'];
      form_set_value($form['domain_301_redirect_domain'], $domain, $form_state);

      if (!domain_301_redirect_check_domain($domain)) {
        form_set_error('domain_301_redirect_enabled', 'Domain 301 redirection can not be enabled as the domain you set does not currently point to this site.');
      }
    }
  }
}