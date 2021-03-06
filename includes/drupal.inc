<?php

/**
 * @file
 * Utility functions related to Drupal.
 */

/**
 * Loads the Drupal autoloader and returns the instance.
 */
function drush_drupal_load_autoloader($drupal_root) {
  static $autoloader = FALSE;

  if (!$autoloader) {
    $autoloader = require $drupal_root .'/autoload.php';
    if ($autoloader === TRUE) {
      // The autoloader was already require(). Assume that Drush and Drupal share an autoloader per
      // "Point autoload.php to the proper vendor directory" - https://www.drupal.org/node/2404989
      $autoloader = drush_get_context('DRUSH_CLASSLOADER');
    }
  }
  return $autoloader;
}

/**
 * Detects the version number of the current Drupal installation,
 * if any. Returns FALSE if there is no current Drupal installation,
 * or it is somehow broken.
 *
 * @return
 *   A string containing the version number of the current
 *   Drupal installation, if any. Otherwise, return FALSE.
 */
function drush_drupal_version($drupal_root = NULL) {
  static $version = FALSE;

  if (!$version) {
    if (($drupal_root != NULL) || ($drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT'))) {
      $bootstrap = \Drush::bootstrapManager()->bootstrapObjectForRoot($drupal_root);
      if ($bootstrap) {
        $version = $bootstrap->get_version($drupal_root);
      }
    }
  }
  return $version;
}

function drush_drupal_cache_clear_all() {
  drush_invoke_process('@self', 'cache-rebuild');
}

/**
 * Returns the Drupal major version number (6, 7, 8 ...)
 */
function drush_drupal_major_version($drupal_root = NULL) {
  $major_version = FALSE;
  if ($version = drush_drupal_version($drupal_root)) {
    $version_parts = explode('.', $version);
    if (is_numeric($version_parts[0])) {
      $major_version = (integer)$version_parts[0];
    }
  }
  return $major_version;
}

/**
 * Build a cache id to store the install_profile for a given site.
 */
function drush_cid_install_profile() {
  return drush_get_cid('install_profile', array(), array(drush_get_context('DRUSH_SELECTED_DRUPAL_SITE_CONF_PATH')));
}
