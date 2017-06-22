// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements the EncryptedStorageAnalysis.

#include "encryptedstorageanalysis.h"

EncryptedStorageAnalysis::EncryptedStorageAnalysis(shared_ptr<Sources> sources)
    : _sources(sources), _threshold(0.9)
{
    _findKeywords = shared_ptr<FindKeywords>(new FindKeywords(sources));
    _findAPICalls = shared_ptr<FindAPICalls>(new FindAPICalls());

    prepareKeywords();
    prepareAPICalls();
}

void EncryptedStorageAnalysis::doPreASTAnalysis()
{

}

void EncryptedStorageAnalysis::doPostASTAnalysis()
{
    _findKeywords->analyzeFilenames();

    _findKeywords->buildFileRating();
    _findAPICalls->buildFileRating();

    for(string file : _sources->sources())
    {
        double rating = 1.0;
        rating = rating * _findKeywords->getFileRating(file);
        rating = rating * _findAPICalls->getFileRating(file);
        rating = cbrt(rating);

        if(rating < 0.99)
        {
            _fileRatings.insert(pair<double,string>(rating,file));
        }
    }
}

vector<unique_ptr<ASTConsumer> > EncryptedStorageAnalysis::getASTConsumers(
        ASTContext *context)
{
    vector<unique_ptr<ASTConsumer> > consumers;

    unique_ptr<ASTConsumer> fkc = unique_ptr<ASTConsumer>(
                new FindKeywordsConsumer(context, _findKeywords));
    consumers.push_back(std::move(fkc));

    return consumers;
}

vector<unique_ptr<RecursiveVisitor> > EncryptedStorageAnalysis::getRecursiveVisitors(
        ASTContext *context)
{
    vector<unique_ptr<RecursiveVisitor> > visitors;

    unique_ptr<RecursiveVisitor> fkv = unique_ptr<RecursiveVisitor>(
                new FindKeywordsVisitor(context, _findKeywords));
    visitors.push_back(std::move(fkv));

    unique_ptr<RecursiveVisitor> fapiv = unique_ptr<RecursiveVisitor>(
                new FindAPIVisitor(context, _findAPICalls, _sources));
    visitors.push_back(std::move(fapiv));

    return visitors;
}

void EncryptedStorageAnalysis::printResults(raw_ostream &out) const
{
    out << "===\nResults for the Encrypted Storage Pattern:\n===\n";
    auto it = _fileRatings.begin();

    while(it != _fileRatings.end() && it->first < _threshold)
    {
        out << "Possible Involvement in the Encrypted Storage Pattern: (Rating: "
            << double_to_string((1.0 - it->first)*100) << " )\n"
            << it->second << "\n";
        _findKeywords->printResultsForFile(it->second, out);
        _findAPICalls->printResultsForFile(it->second, out);
        it++;
    }
    out << "\n\n\n";
}

void EncryptedStorageAnalysis::prepareAPICalls()
{
    //DPAPI
    string dpapi = "DPAPI";
    _findAPICalls->addAPIHeader(dpapi,"dpapi.h");
    _findAPICalls->addAPIFunction("CryptProtectData", dpapi, 2);
    _findAPICalls->addAPIFunction("CryptUnprotectData", dpapi, 2);

    //Libsecret
    string libsecret = "Libsecret";
    _findAPICalls->addAPIHeader(libsecret,"libsecret/secret.h");
    //Libsecret - SecretService
    _findAPICalls->addAPIFunction("secret_service_get", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_get_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_get_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_disconnect", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_open", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_open_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_open_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_get_collections", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_get_flags", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_get_session_algorithms", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_ensure_session", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_ensure_session_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_ensure_session_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_load_collections", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_load_collections_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_load_collections_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_search", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_search_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_search_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_lock", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_lock_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_lock_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_unlock", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_unlock_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_unlock_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_store", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_store_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_store_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_lookup", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_lookup_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_lookup_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_clear", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_clear_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_clear_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_prompt", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_prompt_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_prompt_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_set_alias", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_set_alias_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_set_alias_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_get_collection_gtype", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_service_get_item_gtype", libsecret, 2);
    // Libsecret - SecretCollection
    _findAPICalls->addAPIFunction("secret_collection_for_alias", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_for_alias_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_for_alias_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_load_items", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_load_items_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_load_items_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_create", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_create_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_create_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_search", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_search_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_search_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_delete", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_delete_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_delete_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_get_created", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_get_service", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_get_flags", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_get_items", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_get_label", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_set_label", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_set_label_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_set_label_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_get_locked", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_get_modified", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_collection_refresh", libsecret, 2);
    // Libsecret - SecretValue
    _findAPICalls->addAPIFunction("secret_value_new", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_value_new_full", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_value_get", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_value_get_text", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_value_get_content_type", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_value_ref", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_value_unref", libsecret, 2);
    // Libsecret - SecretItem
    _findAPICalls->addAPIFunction("secret_item_create", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_create_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_create_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_delete", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_delete_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_delete_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_schema_name", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_attributes", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_attributes", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_attributes_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_attributes_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_created", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_label", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_label", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_label_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_label_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_flags", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_locked", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_modified", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_service", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_get_secret", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_load_secret", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_load_secret_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_load_secret_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_load_secrets", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_load_secrets_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_load_secrets_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_secret", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_secret_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_set_secret_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_item_refresh", libsecret, 2);
    // Libsecret - SecretPassword
    _findAPICalls->addAPIFunction("secret_password_store", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_storev", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_store_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_store_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_storev_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_lookup", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_lookupv", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_lookup_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_lookup_nonpageable_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_lookup_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_lookup_nonpageable_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_lookupv_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_lookupv_nonpageable_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_clear", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_clearv", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_clear_finish", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_clear_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_clearv_sync", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_wipe", libsecret, 2);
    _findAPICalls->addAPIFunction("secret_password_free", libsecret, 2);

}

void EncryptedStorageAnalysis::prepareKeywords()
{
    _findKeywords->addKeyword("encrypt", 2, true);
    _findKeywords->addKeyword("decrypt", 2, true);
    _findKeywords->addKeyword("crypt", 2, false);
    _findKeywords->addKeyword("storage", 3, false);
    _findKeywords->addKeyword("protect", 6, true);
    _findKeywords->addKeyword("password", 5, false);
    _findKeywords->addKeyword("file", 10, false);
}

string EncryptedStorageAnalysis::double_to_string(double d) const
{
    stringstream ss;
    ss << std::fixed << std::setprecision(4) << d;
    return ss.str();
}
